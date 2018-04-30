#pragma once

#include <experimental/filesystem>
#include "snmp_client.hpp"
#include "config.h"

namespace phosphor
{
namespace network
{
namespace snmp
{

namespace fs = std::experimental::filesystem;

/** @brief Serialize and persist SNMP manager/client D-Bus object
 *  @param[in] manager - const reference to snmp client/manager object.
 *  @return fs::path - pathname of persisted events file
 */
fs::path serialize(const Client& manager);

/** @brief Deserialze SNMP manager/client info into a D-Bus object
 *  @param[in] path - pathname of persisted manager/client file.
 *  @param[in] manager - reference to snmp client/manager object
 *                       which is the target of deserialization.
 *  @return bool - true if the deserialization was successful, false otherwise.
 */
bool deserialize(const fs::path& path, Client& manager);

} // namespace snmp
} // namespace network
} // namespace phosphor
