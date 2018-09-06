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

ConfManager::ConfManager(sdbusplus::bus::bus& bus, const char* objPath) :
    details::CreateIface(bus, objPath, true),
    dbusPersistentLocation(SNMP_CONF_PERSIST_PATH), bus(bus),
    objectPath(objPath)
{
}

std::string ConfManager::client(std::string address, uint16_t port)
{
    auto clientEntry = this->clients.find(address);
    if (clientEntry != this->clients.end())
    {
        log<level::ERR>("Client already configured"),
            entry("ADDRESS=%s", address.c_str());
        elog<InternalFailure>();
    }
    try
    {
        // just to check whether given address is valid or not.
        resolveAddress(address);
    }
    catch (InternalFailure& e)
    {
        log<level::ERR>("Not a valid address"),
            entry("ADDRESS=%s", address.c_str());
        elog<InvalidArgument>(Argument::ARGUMENT_NAME("Address"),
                              Argument::ARGUMENT_VALUE(address.c_str()));
    }

    std::experimental::filesystem::path objPath;
    objPath /= objectPath;
    objPath /= generateId(address, port);
    // create the D-Bus object
    auto client = std::make_unique<phosphor::network::snmp::Client>(
        bus, objPath.string().c_str(), *this, address, port);
    // save the D-Bus object
    serialize(*client, dbusPersistentLocation);

    this->clients.emplace(address, std::move(client));
    return objPath.string();
}

std::string ConfManager::generateId(const std::string& address, uint16_t port)
{
    std::stringstream hexId;
    std::string hashString = address;
    hashString += std::to_string(port);

    // Only want 8 hex digits.
    hexId << std::hex << ((std::hash<std::string>{}(hashString)) & 0xFFFFFFFF);
    return hexId.str();
}

void ConfManager::deleteSNMPClient(const std::string& address)
{
    auto it = clients.find(address);
    if (it == clients.end())
    {
        log<level::ERR>("Unable to delete the snmp client.",
                        entry("ADDRESS=%s", address.c_str()));
        return;
    }

    std::error_code ec;
    // remove the persistent file
    fs::path fileName = dbusPersistentLocation;
    fileName /=
        it->second->address() + SEPARATOR + std::to_string(it->second->port());

    if (fs::exists(fileName))
    {
        if (!fs::remove(fileName, ec))
        {
            log<level::ERR>("Unable to delete the file",
                            entry("FILE=%s", fileName.c_str()),
                            entry("ERROR=%d", ec.value()));
        }
    }
    else
    {
        log<level::ERR>("File doesn't exist",
                        entry("FILE=%s", fileName.c_str()));
    }
    // remove the D-Bus Object.
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
        auto pos = managerID.find(SEPARATOR);
        auto ipaddress = managerID.substr(0, pos);
        auto port_str = managerID.substr(pos + 1);
        uint16_t port = stoi(port_str, nullptr);

        fs::path objPath = objectPath;
        objPath /= generateId(ipaddress, port);
        auto manager =
            std::make_unique<Client>(bus, objPath.string().c_str(), *this);
        if (deserialize(confFile.path(), *manager))
        {
            manager->emit_object_added();
            this->clients.emplace(ipaddress, std::move(manager));
        }
    }
}

} // namespace snmp
} // namespace network
} // namespace phosphor
