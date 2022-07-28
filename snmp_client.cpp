#include "snmp_client.hpp"

#include "snmp_conf_manager.hpp"
#include "snmp_serialize.hpp"
#include "snmp_util.hpp"

namespace phosphor
{
namespace network
{
namespace snmp
{

Client::Client(sdbusplus::bus_t& bus, const char* objPath, ConfManager& parent,
               const std::string& address, uint16_t port) :
    Ifaces(bus, objPath, Ifaces::action::defer_emit),
    id(std::stol(std::filesystem::path(objPath).filename())), parent(parent)
{
    this->address(address);
    this->port(port);

    // Emit deferred signal.
    emit_object_added();
}

std::string Client::address(std::string value)
{
    if (value == Ifaces::address())
    {
        return value;
    }

    parent.checkClientConfigured(value, port());

    value = resolveAddress(value);
    auto addr = Ifaces::address(value);
    serialize(id, *this, parent.dbusPersistentLocation);
    return addr;
}

uint16_t Client::port(uint16_t value)
{
    if (value == Ifaces::port())
    {
        return value;
    }

    parent.checkClientConfigured(address(), value);

    auto port = Ifaces::port(value);
    serialize(id, *this, parent.dbusPersistentLocation);
    return port;
}

void Client::delete_()
{
    parent.deleteSNMPClient(id);
}

Id Client::getClientId()
{
    return this->id;
}

} // namespace snmp
} // namespace network
} // namespace phosphor
