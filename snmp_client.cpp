#include "snmp_client.hpp"
#include "snmp_manager.hpp"

namespace phosphor
{
namespace network
{
namespace snmp
{

Client::Client(sdbusplus::bus::bus& bus,
               const char* objPath,
               ConfManager& parent,
               const std::string& ipaddress,
               uint16_t iport) :
    Ifaces(bus, objPath, true),
    parent(parent)
{
    this->address(ipaddress);
    this->port(iport);

    // Emit deferred signal.
    emit_object_added();
}


void Client::delete_()
{
    // TODO: Implement in the later commit.
}

} //namespace snmp
} //namespace network
} //namespace phosphor
