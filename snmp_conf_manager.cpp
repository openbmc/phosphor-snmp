#include "config.h"

#include "snmp_conf_manager.hpp"

#include "snmp_serialize.hpp"
#include "snmp_util.hpp"
#include "xyz/openbmc_project/Common/error.hpp"
#include "xyz/openbmc_project/Common/event.hpp"

#include <arpa/inet.h>

#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/lg2.hpp>

#include <filesystem>

namespace phosphor
{
namespace network
{
namespace snmp
{

using namespace phosphor::logging;
using namespace sdbusplus::xyz::openbmc_project::Common::Error;
using Argument = xyz::openbmc_project::Common::InvalidArgument;
using ObjectAlreadyExists =
    sdbusplus::error::xyz::openbmc_project::Common::ObjectAlreadyExists;

ConfManager::ConfManager(sdbusplus::bus_t& bus, const char* objPath) :
    details::CreateIface(bus, objPath,
                         details::CreateIface::action::defer_emit),
    dbusPersistentLocation(SNMP_CONF_PERSIST_PATH), bus(bus),
    objectPath(objPath)
{}

std::string ConfManager::client(std::string address, uint16_t port)
{
    // will throw exception if it is already configured.
    checkClientConfigured(address, port);

    try
    {
        // just to check whether given address is valid or not.
        resolveAddress(address);
    }
    catch (const InternalFailure& e)
    {
        lg2::error("{ADDRESS} is not a valid address", "ADDRESS", address);
        elog<InvalidArgument>(Argument::ARGUMENT_NAME("Address"),
                              Argument::ARGUMENT_VALUE(address.c_str()));
    }

    auto nextId = lastClientId + 1;
    // create the D-Bus object
    std::filesystem::path objPath;
    objPath /= objectPath;
    objPath /= std::to_string(nextId);

    auto client = std::make_unique<phosphor::network::snmp::Client>(
        bus, objPath.string().c_str(), *this, address, port);

    // save the D-Bus object
    serialize(nextId, *client, dbusPersistentLocation);

    this->clients.emplace(nextId, std::move(client));
    lastClientId = nextId;
    return objPath.string();
}

void ConfManager::checkClientConfigured(const std::string& address,
                                        uint16_t port)
{
    if (address.empty())
    {
        lg2::error("{ADDRESS} is not a valid address", "ADDRESS", address);
        elog<InvalidArgument>(Argument::ARGUMENT_NAME("ADDRESS"),
                              Argument::ARGUMENT_VALUE(address.c_str()));
    }

    for (const auto& val : clients)
    {
        if (val.second.get()->address() == address &&
            val.second.get()->port() == port)
        {
            std::filesystem::path objPath = objectPath;
            objPath /= std::to_string(val.first);

            lg2::error("Client already exists at {OBJECT_PATH}", "OBJECT_PATH",
                       objPath.string());

            throw ObjectAlreadyExists("OBJECT_PATH",
                                      sdbusplus::object_path(objPath.string()));
        }
    }
}

void ConfManager::deleteSNMPClient(Id id)
{
    auto it = clients.find(id);
    if (it == clients.end())
    {
        lg2::error("Unable to delete the snmp client: {ID}", "ID", id);
        return;
    }

    std::error_code ec;
    // remove the persistent file
    fs::path fileName = dbusPersistentLocation;
    fileName /= std::to_string(id);

    if (fs::exists(fileName))
    {
        if (!fs::remove(fileName, ec))
        {
            lg2::error("Unable to delete {FILE}: {EC}", "FILE", fileName, "EC",
                       ec.value());
            return;
        }
    }
    else
    {
        lg2::error("{FILE} doesn't exist", "FILE", fileName);
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
        Id idNum = std::stol(managerID);

        fs::path objPath = objectPath;
        objPath /= managerID;
        auto manager =
            std::make_unique<Client>(bus, objPath.string().c_str(), *this);
        if (deserialize(confFile.path(), *manager))
        {
            manager->emit_object_added();
            this->clients.emplace(idNum, std::move(manager));
            if (idNum > lastClientId)
            {
                lastClientId = idNum;
            }
        }
    }
}

} // namespace snmp
} // namespace network
} // namespace phosphor
