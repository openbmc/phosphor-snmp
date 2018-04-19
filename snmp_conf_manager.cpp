#include "config.h"
#include "snmp_conf_manager.hpp"
#include <phosphor-logging/log.hpp>

#include <experimental/filesystem>

namespace phosphor
{
namespace network
{
namespace snmp
{

using namespace phosphor::logging;

ConfManager::ConfManager(sdbusplus::bus::bus& bus, const char* objPath) :
    details::CreateIface(bus, objPath, true), bus(bus), objectPath(objPath)
{
}

void ConfManager::client(std::string address, uint16_t port)
{
    auto clientEntry = this->clients.find(address);
    if (clientEntry == this->clients.end())
    {
        std::experimental::filesystem::path objPath;
        objPath /= objectPath;
        objPath /= generateId(address, port);

        this->clients.emplace(
            address, std::make_unique<phosphor::network::snmp::Client>(
                         bus, objPath.string().c_str(), *this, address, port));
    }
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
    this->clients.erase(it);
}

} // namespace snmp
} // namespace network
} // namespace phosphor
