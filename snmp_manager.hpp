#pragma once

#include "snmp_client.hpp"

#include <xyz/openbmc_project/Network/Client/Create/server.hpp>
#include <sdbusplus/bus.hpp>

#include <string>

namespace phosphor
{
namespace network
{
namespace snmp
{

using ClientList = std::map<std::string, std::unique_ptr<Client>>;

namespace details
{

using CreateIface = sdbusplus::server::object::object <
                    sdbusplus::xyz::openbmc_project::Network::Client::server::Create >;

} // namespace details


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

        /** @brief Constructor to put object onto bus at a dbus path.
         *  @param[in] bus - Bus to attach to.
         *  @param[in] objPath - Path to attach at.
         */
        ConfManager(sdbusplus::bus::bus& bus, const char* objPath);

        void client(std::string ipaddress, uint16_t port) override;

        /* @brief delete the dbus object of the given ipaddress.
         * @param[in] ipaddress - IP address.
         */
        void deleteSNMPClient(const std::string& ipaddress);

    protected:

        /** @brief generates the id by doing hash of ipaddress,port
         *  @param[in] ipaddress - IP address.
         *  @param[in] port - network port.
         *  @return hash string.
         */
        static std::string generateId(const std::string& ipaddress,
                                      uint16_t port);
    private:
        /** @brief Persistent sdbusplus DBus bus connection. */
        sdbusplus::bus::bus& bus;

        /** @brief Path of Object. */
        std::string objectPath;

        /** @brief Persistent map of IPAddress dbus objects and their names */
        ClientList clients;

};

} // namespace snmp
} // namespace network
} // namespace phosphor
