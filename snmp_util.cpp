#include "snmp_util.hpp"

#include "xyz/openbmc_project/Common/error.hpp"

#include <arpa/inet.h>
#include <netdb.h>

#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/lg2.hpp>

#include <string>

namespace phosphor
{

using namespace phosphor::logging;
using namespace sdbusplus::xyz::openbmc_project::Common::Error;

ObjectValueTree getManagedObjects(sdbusplus::bus_t& bus,
                                  const std::string& service,
                                  const std::string& objPath)
{
    ObjectValueTree interfaces;

    auto method = bus.new_method_call(service.c_str(), objPath.c_str(),
                                      "org.freedesktop.DBus.ObjectManager",
                                      "GetManagedObjects");

    try
    {
        auto reply = bus.call(method);
        reply.read(interfaces);
    }
    catch (const sdbusplus::exception_t& e)
    {
        lg2::error("Failed to get managed objects: {PATH}", "PATH", objPath);
        elog<InternalFailure>();
    }

    return interfaces;
}

namespace network
{

std::string resolveAddress(const std::string& address)
{
    addrinfo hints{};
    addrinfo* addr = nullptr;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;

    auto result = getaddrinfo(address.c_str(), nullptr, &hints, &addr);
    if (result)
    {
        lg2::error("getaddrinfo failed {ADDRESS}: {RC}", "ADDRESS", address,
                   "RC", result);
        elog<InternalFailure>();
    }

    AddrPtr addrPtr{addr};
    addr = nullptr;

    char ipaddress[INET6_ADDRSTRLEN]{0};
    result = getnameinfo(addrPtr->ai_addr, addrPtr->ai_addrlen, ipaddress,
                         sizeof(ipaddress), nullptr, 0, NI_NUMERICHOST);
    if (result)
    {
        lg2::error("getnameinfo failed {ADDRESS}: {RC}", "ADDRESS", address,
                   "RC", result);
        elog<InternalFailure>();
    }

    unsigned char buf[sizeof(struct in6_addr)];
    int isValid = inet_pton(AF_INET, ipaddress, buf);
    if (isValid < 0)
    {
        lg2::error("Invalid address {ADDRESS}: {RC}", "ADDRESS", address, "RC",
                   isValid);
        elog<InternalFailure>();
    }
    if (isValid == 0)
    {
        int isValid6 = inet_pton(AF_INET6, ipaddress, buf);
        if (isValid6 < 1)
        {
            lg2::error("Invalid address {ADDRESS}: {RC}", "ADDRESS", address,
                       "RC", isValid);
            elog<InternalFailure>();
        }
    }

    return ipaddress;
}

namespace snmp
{

static constexpr auto busName = "xyz.openbmc_project.Network.SNMP";
static constexpr auto root = "/xyz/openbmc_project/network/snmp/manager";
static constexpr auto clientIntf = "xyz.openbmc_project.Network.Client";

/** @brief Gets the sdbus object for this process.
 *  @return the bus object.
 */
static auto& getBus()
{
    static auto bus = sdbusplus::bus::new_default();
    return bus;
}

std::vector<std::string> getManagers()
{
    std::vector<std::string> managers;
    auto& bus = getBus();
    try
    {
        auto objTree = phosphor::getManagedObjects(bus, busName, root);
        for (const auto& [_, intfMap] : objTree)
        {
            if (!intfMap.contains(clientIntf))
            {
                continue;
            }

            auto& snmpClientProps = intfMap.at(clientIntf);
            auto& address =
                std::get<std::string>(snmpClientProps.at("Address"));
            auto& port = std::get<uint16_t>(snmpClientProps.at("Port"));
            auto ipaddress = phosphor::network::resolveAddress(address);
            auto mgr = std::move(ipaddress);
            if (port > 0)
            {
                mgr += ":";
                mgr += std::to_string(port);
            }
            managers.push_back(mgr);
        }
    }
    catch (const std::exception& e)
    {
        lg2::error("Invalid address: {ERROR}", "ERROR", e);
    }

    return managers;
}

} // namespace snmp
} // namespace network
} // namespace phosphor
