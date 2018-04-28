#include "snmp_util.hpp"
#include "xyz/openbmc_project/Common/error.hpp"

#include <arpa/inet.h>

#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/log.hpp>

namespace phosphor
{

using namespace phosphor::logging;
using namespace sdbusplus::xyz::openbmc_project::Common::Error;

ObjectValueTree getManagedObjects(sdbusplus::bus::bus& bus,
                                  const std::string& service,
                                  const std::string& objPath)
{
    ObjectValueTree interfaces;

    auto method = bus.new_method_call(
                      service.c_str(),
                      objPath.c_str(),
                      "org.freedesktop.DBus.ObjectManager",
                      "GetManagedObjects");

    auto reply = bus.call(method);

    if (reply.is_method_error())
    {
        log<level::ERR>("Failed to get managed objects",
                        entry("PATH=%s", objPath.c_str()));
        elog<InternalFailure>();
    }

    reply.read(interfaces);
    return interfaces;
}

namespace network
{

bool isValidIP(int addressFamily, const std::string& address)
{
    unsigned char buf[sizeof(struct in6_addr)];

    return inet_pton(addressFamily, address.c_str(), buf) > 0;
}

namespace snmp
{

static constexpr auto busName = "xyz.openbmc_project.Network.SNMP";
static constexpr auto root = "/xyz/openbmc_project/network/snmp/manager";
static constexpr auto clientIntf =
    "xyz.openbmc_project.Network.Client";

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
    auto objTree = phosphor::getManagedObjects(bus, busName, root);
    for (const auto& objIter : objTree)
    {
        try
        {
            auto& intfMap = objIter.second;
            auto& snmpClientProps = intfMap.at(clientIntf);
            auto& ipaddress =
                snmpClientProps.at("Address").get<std::string>();
            auto& port = snmpClientProps.at("Port").get<uint16_t>();
            auto mgr = std::move(ipaddress);
            if (port > 0)
            {
                mgr += ":";
                mgr += std::to_string(port);
            }
            managers.push_back(mgr);
        }
        catch (const std::exception& e)
        {
            log<level::ERR>(e.what());
        }
    }
    return managers;
}


} // namespace snmp
} // namespace network
} // namespace phosphor
