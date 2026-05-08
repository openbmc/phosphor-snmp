#pragma once

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server.hpp>
#include <sdbusplus/vtable.hpp>

#include <filesystem>
#include <string>

namespace phosphor
{
namespace network
{
namespace snmp
{

namespace fs = std::filesystem;

constexpr auto communityStrPersistPath = "/var/lib/phosphor-snmp/community/";
constexpr auto communityStrInterface =
    "xyz.openbmc_project.Network.SNMP.CommunityString";

/** @class CommunityManager
 *  @brief Manages SNMP community string configuration.
 *
 *  Exposes the CommunityString property on D-Bus at the
 *  /xyz/openbmc_project/network/snmp/manager path under the
 *  xyz.openbmc_project.Network.SNMP.CommunityString interface.
 */
class CommunityManager
{
  public:
    CommunityManager() = delete;
    CommunityManager(const CommunityManager&) = delete;
    CommunityManager& operator=(const CommunityManager&) = delete;
    CommunityManager(CommunityManager&&) = delete;
    CommunityManager& operator=(CommunityManager&&) = delete;
    ~CommunityManager() = default;

    /** @brief Constructor to put object onto bus at a D-Bus path.
     *  @param[in] bus - Bus to attach to.
     *  @param[in] objPath - Path to attach at.
     */
    CommunityManager(sdbusplus::bus_t& bus, const char* objPath);

    /** @brief Get the community string value.
     *  @return The current community string.
     */
    std::string getCommunityString() const;

    /** @brief Set the community string value.
     *  @param[in] value - New community string value.
     */
    void setCommunityString(const std::string& value);

  private:
    /** @brief sdbusplus DBus bus object. */
    sdbusplus::bus_t& bus;

    /** @brief The D-Bus object path. */
    std::string objectPath;

    /** @brief The current community string value. */
    std::string communityString;

    /** @brief The D-Bus interface object. */
    sdbusplus::server::interface_t iface;

    /** @brief Persist the community string to filesystem. */
    void persist() const;

    /** @brief Restore community string from persistent storage. */
    void restore();

    /** @brief D-Bus property get handler. */
    static int propertyGet(sd_bus* bus, const char* path, const char* interface,
                           const char* property, sd_bus_message* reply,
                           void* userdata, sd_bus_error* error);

    /** @brief D-Bus property set handler. */
    static int propertySet(sd_bus* bus, const char* path, const char* interface,
                           const char* property, sd_bus_message* value,
                           void* userdata, sd_bus_error* error);

    /** @brief The vtable for the CommunityString interface. */
    static const sdbusplus::vtable_t vtable[];
};

} // namespace snmp
} // namespace network
} // namespace phosphor
