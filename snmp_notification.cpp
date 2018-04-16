#include "snmp_notification.hpp"

#include <phosphor-logging/elog-errors.hpp>
#include <phosphor-logging/log.hpp>

#include "xyz/openbmc_project/Common/error.hpp"

namespace phosphor
{
namespace network
{
namespace snmp
{

using namespace phosphor::logging;
using namespace sdbusplus::xyz::openbmc_project::Common::Error;

using snmpSessionPtr =
    std::unique_ptr<netsnmp_session, decltype(&::snmp_close)>;

bool Notification::addPDUVar(netsnmp_pdu& pdu, const oid& objID,
                             size_t objIDLen, u_char type, Value val)
{
    netsnmp_variable_list* varList = nullptr;
    switch (type)
    {
        case ASN_INTEGER:
        {
            auto ltmp = val.get<long>();
            varList = snmp_pdu_add_variable(&pdu, &objID, objIDLen, type,
                                            &ltmp, sizeof(ltmp));
        }
        break;
        case ASN_UNSIGNED:
        {
            auto ltmp = val.get<unsigned long int>();
            varList = snmp_pdu_add_variable(&pdu, &objID, objIDLen, type,
                                            &ltmp, sizeof(ltmp));
        }
        break;
        case ASN_OPAQUE_U64:
        {
            auto ltmp = val.get<uint64_t>();
            struct counter64 c64tmp;
            if (read64(&c64tmp, (char*)ltmp))
            {
                varList =  snmp_pdu_add_variable(&pdu, &objID, objIDLen, type,
                                                 &c64tmp, sizeof(c64tmp));
            }
        }
        break;
        case ASN_OCTET_STR:
        {
            auto value = val.get<std::string>();
            varList = snmp_pdu_add_variable(&pdu, &objID, objIDLen, type,
                                            value.c_str(), value.length());
        }
        break;
    }
    return (varList == nullptr ? false : true);
}

void Notification::sendTrap()
{
    log<level::DEBUG>("Sending SNMP Trap");

    constexpr auto comm = "public";
    constexpr auto localHost  = "127.0.0.1";
    netsnmp_session session { 0 };

    snmp_sess_init(&session);

    init_snmp("snmpapp");

    //TODO: https://github.com/openbmc/openbmc/issues/3145
    session.version = SNMP_VERSION_2c;
    session.community = (u_char*)comm;
    session.community_len = strlen(comm);
    session.callback = nullptr;
    session.callback_magic = nullptr;

    // TODO:- get it from settings D-bus object.
    session.peername = const_cast<char*>(localHost);

    // create the session
    auto ss = snmp_add(&session,
                       netsnmp_transport_open_client("snmptrap", session.peername),
                       nullptr, nullptr);
    if (!ss)
    {
        log<level::ERR>("Unable to get the snmp session.",
                        entry("SNMPMANAGER=%s",session.peername));
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


    auto trapID =  getTrapOID();

    if (!snmp_pdu_add_variable(pdu, SNMPTrapOID,
                               sizeof(SNMPTrapOID) / sizeof(oid),
                               ASN_OBJECT_ID, trapID, sizeof(trapID)))
    {
        log<level::ERR>("Failed to add the SNMP var");
        snmp_free_pdu(pdu);
        elog<InternalFailure>();
    }

    auto objectList = getFieldOIDList();

    for (const auto& object : objectList)
    {
        if (!addPDUVar(*pdu, *(std::get<0>(object).data()),
                       std::get<1>(object),
                       std::get<2>(object), std::get<3>(object)))
        {
            log<level::ERR>("Failed to add the SNMP var");
            snmp_free_pdu(pdu);
            elog<InternalFailure>();
        }
    }

    // pdu is freed by snmp_send
    if (!snmp_send(sessionPtr.get(), pdu))
    {
        log<level::ERR>("Failed to send the snmp trap.");
        elog<InternalFailure>();

    }

    log<level::DEBUG>("Sent SNMP Trap");
}

} // namespace snmp
} // namespace network
} // namespace phosphor
