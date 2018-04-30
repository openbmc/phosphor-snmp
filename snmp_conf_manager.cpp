#include "config.h"
#include "snmp_conf_manager.hpp"
#include "snmp_serialize.hpp"
#include "snmp_util.hpp"
#include "xyz/openbmc_project/Common/error.hpp"

#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/log.hpp>

#include <experimental/filesystem>

#include <arpa/inet.h>

namespace phosphor
{
namespace network
{
namespace snmp
{

using namespace phosphor::logging;
using namespace sdbusplus::xyz::openbmc_project::Common::Error;
using Argument = xyz::openbmc_project::Common::InvalidArgument;

ConfManager::ConfManager(sdbusplus::bus::bus& bus, const char* objPath):
    details::CreateIface(bus, objPath, true),
    dbusPersistentLocation(SNMP_CONF_PERSIST_PATH),
    bus(bus),
    objectPath(objPath) {}


void ConfManager::client(std::string ipaddress, uint16_t port,
                         IPProtocol addressType)
{

    int addressFamily = (addressType == IPProtocol::IPv4) ? AF_INET : AF_INET6;

    if (!isValidIP(addressFamily, ipaddress))
    {
        log<level::ERR>("Not a valid IP address"),
                        entry("ADDRESS=%s", ipaddress.c_str());
        elog<InvalidArgument>(Argument::ARGUMENT_NAME("IPaddress"),
                              Argument::ARGUMENT_VALUE(ipaddress.c_str()));
    }

    auto clientEntry = this->clients.find(ipaddress);
    if (clientEntry == this->clients.end())
    {
        std::experimental::filesystem::path objPath;
        objPath /= objectPath;
        objPath /= generateId(ipaddress, port);

        auto client = std::make_unique<phosphor::network::snmp::Client>(
                        bus,
                        objPath.string().c_str(),
                        *this,
                        ipaddress,
                        port,
                        addressType);

        // save the D-Bus object
        serialize(*client, dbusPersistentLocation);

        this->clients.emplace(ipaddress,std::move(client));
    }
}

std::string ConfManager::generateId(const std::string& ipaddress,
                                    uint16_t port)
{
    std::stringstream hexId;
    std::string hashString = ipaddress;
    hashString += std::to_string(port);

    // Only want 8 hex digits.
    hexId << std::hex << ((std::hash<std::string> {}(
                               hashString)) & 0xFFFFFFFF);
    return hexId.str();
}

void ConfManager::deleteSNMPClient(const std::string& ipaddress)
{
    auto it = clients.find(ipaddress);
    if (it == clients.end())
    {
        log<level::ERR>("Unable to delete the snmp client.",
                        entry("IP=%s", ipaddress.c_str()));
        return;
    }
    this->clients.erase(it);
}

void ConfManager::restoreClients()
{
    if (!fs::exists(dbusPersistentLocation) ||
         fs::is_empty(dbusPersistentLocation))
    {
        return;
    }

    for (auto& confFile :
         fs::recursive_directory_iterator(dbusPersistentLocation))
    {
        if (!fs::is_regular_file(confFile))
        {
            continue;
        }

        auto managerID = confFile.path().filename().string();
        auto pos = managerID.find(SEPRATOR);
        auto ipaddress = managerID.substr(0, pos);
        auto port_str = managerID.substr(pos + 1);
        uint16_t port = stoi(port_str, nullptr);

        fs::path objPath = objectPath;
        objPath /= generateId(ipaddress, port);
        auto manager = std::make_unique<Client>(bus, objPath.string().c_str(), *this);
        if (deserialize(confFile.path(), *manager))
        {
            manager->emit_object_added();
            this->clients.emplace(ipaddress, std::move(manager));
        }

    }
}


}//namespace snmp
}//namespace network
}//namespace phosphor
