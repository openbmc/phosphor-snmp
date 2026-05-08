#pragma once

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/object.hpp>
#include <xyz/openbmc_project/Network/SNMP/CommunityString/server.hpp>

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

using CommunityIface = sdbusplus::server::object_t<
    sdbusplus::xyz::openbmc_project::Network::SNMP::server::CommunityString>;

/** @class CommunityManager
 *  @brief Manages SNMP community string configuration.
 *
 *  Exposes the CommunityString property on D-Bus at the
 *  /xyz/openbmc_project/network/snmp/manager path under the
 *  xyz.openbmc_project.Network.SNMP.CommunityString interface.
 */
class CommunityManager : public CommunityIface
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

    /** @brief Override the CommunityString property setter.
     *  @param[in] value - New community string value.
     *  @return The updated community string.
     */
    std::string communityString(std::string value) override;

    using CommunityIface::communityString;

  private:
    /** @brief Persist the community string to filesystem. */
    void persist() const;

    /** @brief Restore community string from persistent storage. */
    void restore();
};

} // namespace snmp
} // namespace network
} // namespace phosphor
