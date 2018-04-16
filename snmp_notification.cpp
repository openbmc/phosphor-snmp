#include "snmp_notification.hpp"

#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/log.hpp>

#include "xyz/openbmc_project/Common/error.hpp"

namespace phosphor
{
namespace snmp
{

using namespace phosphor::logging;
using namespace sdbusplus::xyz::openbmc_project::Common::Error;

using snmpSessionPtr = std::unique_ptr<netsnmp_session, decltype(&::snmp_close)>;

void Notification::sendTrap()
{
    log<level::INFO>("Sending SNMP Trap");

    constexpr auto comm = "public";
    netsnmp_session session;

    snmp_sess_init(&session);

    session.version = SNMP_VERSION_2c;
    session.community = (u_char*)comm;
    session.community_len = strlen((char*)session.community);
    session.callback = nullptr;
    session.callback_magic = nullptr;

    // TODO:- get it from settings D-bus object.
    session.peername = (char*)"127.0.0.1";

    init_snmp("snmpapp");

    // create the session
    auto ss = snmp_add(&session,
                       netsnmp_transport_open_client("snmptrap", session.peername),
                       NULL, NULL);
    if (!ss)
    {
        log<level::ERR>("Unable to get the snmp session.");
        elog<InternalFailure>();
    }

    // Wrap the raw pointer in RAII
    snmpSessionPtr sessionPtr(ss, &::snmp_close);

    ss = nullptr;

    auto pdu = snmp_pdu_create(SNMP_MSG_TRAP2);
    if (!pdu)
    {
        log<level::ERR>("Failed to create notification PDU");
        elog<InternalFailure>();
    }

    pdu->trap_type = SNMP_TRAP_ENTERPRISESPECIFIC;

    auto trapType =  getNotificationType();

    if (snmp_add_var(pdu, SNMPTrapOID.data(),
                     OID_LENGTH(SNMPTrapOID.data()), 'o', trapType.c_str()))
    {
        log<level::ERR>("Failed to add the SNMP var");
        printf("Failed to add the SNMP var***\n");
        snmp_free_pdu(pdu);
        elog<InternalFailure>();
    }

    auto objectList = getFieldOIDList();

    for (const auto& object : objectList)
    {
       if (snmp_add_var(pdu, std::get<0>(object).data(),
                        OID_LENGTH(std::get<0>(object).data()),
                        std::get<1>(object),std::get<2>(object).c_str()))
       {
           log<level::ERR>("Failed to add the SNMP var");
           printf("Failed to add the SNMP var>>>>>\n");
           snmp_free_pdu(pdu);
           elog<InternalFailure>();
       }
    }

    // pdu is freed by snmp_send
    if (!snmp_send(sessionPtr.get(), pdu))
    {
        printf("Failed to send the snmp trap\n");
        log<level::ERR>("Failed to send the snmp trap.");
        elog<InternalFailure>();

    }

    log<level::INFO>("Sent SNMP Trap");
}

} // namespace snmp
} // namespace phosphor
