#pragma once

#include "snmp_client.hpp"

#include <xyz/openbmc_project/Network/Client/Create/server.hpp>
#include "xyz/openbmc_project/Network/Client/server.hpp"
#include <sdbusplus/bus.hpp>

#include <experimental/filesystem>
#include <string>

namespace phosphor
{
namespace network
{
namespace snmp
{

using IPAddress = std::string;
using ClientList = std::map<IPAddress, std::unique_ptr<Client>>;
namespace fs = std::experimental::filesystem;
using IPProtocol =
    sdbusplus::xyz::openbmc_project::Network::server::Client::IPProtocol;

namespace details
{

using CreateIface = sdbusplus::server::object::object<
    sdbusplus::xyz::openbmc_project::Network::Client::server::Create>;

} // namespace details

class TestSNMPConfManager;
/** @class Manager
 *  @brief OpenBMC SNMP config  implementation.
 */
class ConfManager : public details::CreateIface
{
  public:
    ConfManager() = delete;
    ConfManager(const ConfManager&) = delete;
    ConfManager& operator=(const ConfManager&) = delete;
    ConfManager(ConfManager&&) = delete;
    ConfManager& operator=(ConfManager&&) = delete;
    virtual ~ConfManager() = default;

    /** @brief Constructor to put object onto bus at a D-Bus path.
     *  @param[in] bus - Bus to attach to.
     *  @param[in] objPath - Path to attach at.
     */
    ConfManager(sdbusplus::bus::bus& bus, const char* objPath);

    /** @brief Function to create snmp manager details D-Bus object.
     *  @param[in] address- IP address/Hostname.
     *  @param[in] port - network port.
     *  @param[in] addressType - addressType could be Ipv4/Ipv6.
     */
    void client(std::string ipaddress, uint16_t port,
                IPProtocol addressType) override;

    /* @brief delete the D-Bus object of the given ipaddress.
     * @param[in] address - IP address/Hostname.
     */
    void deleteSNMPClient(const std::string& address);

    /** @brief Construct manager/client D-Bus objects from their persisted
     *         representations.
     */
    void restoreClients();

    /** @brief location of the persisted D-Bus object.*/
    fs::path dbusPersistentLocation;

  protected:
    /** @brief generates the id by doing hash of ipaddress, port
     *  @param[in] address - IP address/Hostname.
     *  @param[in] port - network port.
     *  @return hash string.
     */
    static std::string generateId(const std::string& address, uint16_t port);

  private:
    /** @brief sdbusplus DBus bus object. */
    sdbusplus::bus::bus& bus;

    /** @brief Path of Object. */
    std::string objectPath;

    /** @brief map of IPAddress dbus objects and their names */
    ClientList clients;

    friend class TestSNMPConfManager;
};

} // namespace snmp
} // namespace network
} // namespace phosphor
