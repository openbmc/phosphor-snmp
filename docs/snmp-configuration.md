# SNMP trap

Presently BMC sends the SNMP trap in the case of any error log generated on the BMC.

Mib for the error log is defined at the following location.

https://github.com/openbmc/phosphor-snmp/mibs

There are couple of ways through which user can configure the Dbus-monitor to generate the SNMP trap.

* Interface added signal on the D-Bus object.
* Properties changed signal on the D-Bus object.

Other openbmc application can still send the SNMP trap as per their need,They need to
use the API exposed by the libsnmp.

Phosphor-snmp repository exposes the following lib and the D-Bus service

libsnmp: Exposes the API to send the SNMP trap
phosphor-snmp-conf: Used for configuring the SNMP manager(Client).

# SNMP Manager Configuration

User can configure multiple SNMP managers for sending the trap.

## Add the SNMP Manager

### Rest

e.g.
curl -c cjar -b cjar -k -H "Content-Type: application/json" -X  POST -d '{"data": ["<snmp manager ip>", 186, "xyz.openbmc_project.Network.Client.IPProtocol.IPv4"] }' https://<hostname/ip>/xyz/openbmc_project/network/snmp/manager/action/Client

### busctl

e.g.
busctl call  xyz.openbmc_project.Network.SNMP /xyz/openbmc_project/network/snmp/manager xyz.openbmc_project.Network.Client.Create Client sqs "<snmp manager ip>" 162 "xyz.openbmc_project.Network.Client.IPProtocol.IPv4"

## Delete the SNMP Manager

### Rest
curl -c cjar -b cjar -k -H "Content-Type: application/json" -X DELETE https://wsbmc002/xyz/openbmc_project/network/snmp/manager/<ObjectID>

### busctl
e.g.
busctl call xyz.openbmc_project.Network.SNMP /xyz/openbmc_project/network/snmp/manager/<id> xyz.openbmc_project.Object.Delete Delete

