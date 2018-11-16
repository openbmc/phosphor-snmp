#include "snmp_client.hpp"
#include "snmp_conf_manager.hpp"

namespace phosphor
{
namespace network
{
namespace snmp
{

Client::Client(sdbusplus::bus::bus& bus, const char* objPath,
               ConfManager& parent, const std::string& address, uint16_t port) :
    Ifaces(bus, objPath, true),
    id(std::stol(std::experimental::filesystem::path(objPath).filename())),
    parent(parent)
{
    this->address(address);
    this->port(port);

    // Emit deferred signal.
    emit_object_added();
}

void Client::delete_()
{
    parent.deleteSNMPClient(id);
}

} // namespace snmp
} // namespace network
} // namespace phosphor
