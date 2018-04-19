#pragma once

#include <stdint.h>

#include <map>
#include <string>

#include <sdbusplus/server.hpp>

namespace phosphor
{

using DbusInterface = std::string;
using DbusProperty = std::string;

using Value =
    sdbusplus::message::variant<bool, uint8_t, int16_t, uint16_t, int32_t,
                                uint32_t, int64_t, uint64_t, std::string>;

using PropertyMap = std::map<DbusProperty, Value>;

using DbusInterfaceMap = std::map<DbusInterface, PropertyMap>;

using ObjectValueTree =
    std::map<sdbusplus::message::object_path, DbusInterfaceMap>;

/** @brief Gets all managed objects associated with the given object
 *         path and service.
 *  @param[in] bus - D-Bus Bus Object.
 *  @param[in] service - D-Bus service name.
 *  @param[in] objPath - D-Bus object path.
 *  @return On success returns the map of name value pair.
 */
ObjectValueTree getManagedObjects(sdbusplus::bus::bus& bus,
                                  const std::string& service,
                                  const std::string& objPath);

namespace network
{
namespace snmp
{

/** @brief Gets all the snmp manager info.
 *  @return the list of manager info in the format
 *          of ipaddress:port
 */
std::vector<std::string> getManagers();

} // namespace snmp
} // namespace network

} // namespace phosphor
