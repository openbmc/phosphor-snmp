#pragma once

#include "xyz/openbmc_project/Network/Client/server.hpp"
#include "xyz/openbmc_project/Object/Delete/server.hpp"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/object.hpp>

#include <string>

namespace phosphor
{
namespace network
{
namespace snmp
{

class ConfManager;

using Ifaces =
    sdbusplus::server::object::object<
        sdbusplus::xyz::openbmc_project::Network::server::Client,
        sdbusplus::xyz::openbmc_project::Object::server::Delete>;

using IPProtocol =
        sdbusplus::xyz::openbmc_project::Network::server::Client::IPProtocol;

/** @class Client
 *  @brief represents the snmp client configuration
 *  @details A concrete implementation for the
 *  xyz.openbmc_project.Network.Client Dbus interface.
 */
class Client : public Ifaces
{
    public:
        Client() = delete;
        Client(const Client&) = delete;
        Client& operator=(const Client&) = delete;
        Client(Client&&) = delete;
        Client& operator=(Client &&) = delete;
        virtual ~Client() = default;

        /** @brief Constructor to put object onto bus at a dbus path.
         *  @param[in] bus - Bus to attach to.
         *  @param[in] objPath - Path to attach at.
         *  @param[in] parent - Parent D-bus Object.
         *  @param[in] ipAddress - ipadress.
         *  @param[in] port - network port.
         *  @param[in] addressType - Address type(IPv4/IPv6).
         */
        Client(sdbusplus::bus::bus& bus,
               const char* objPath,
               ConfManager& parent,
               const std::string& ipAddress,
               uint16_t port,
               IPProtocol addressType);

        /** @brief Delete this d-bus object.
         */
        void delete_() override;

    private:
        /** @brief Parent D-Bus Object. */
        ConfManager& parent;

};

} // namespace snmp
} // namespace network
} // namespace phosphor
