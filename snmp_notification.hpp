/**
 * @brief SNMP Error Notification class.
 *
 * This file is part of phosphor-snmp project.
 *
 * Copyright (c) 2018 IBM Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Note: In near future this file will be autogenerated by the custom parser.
 *
 */

#pragma once

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <sdbusplus/server.hpp>

#include <experimental/any>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace any_ns = std::experimental;

namespace phosphor
{
namespace network
{
namespace snmp
{

using OID = std::array<oid, MAX_OID_LEN>;
using OID_LEN = size_t;
using Type = u_char;

using Value = sdbusplus::message::variant<uint32_t, uint64_t,
                                          int32_t, std::string>;
// Generic snmp trap ID
oid SNMPTrapOID[] = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};

using Object = std::tuple<OID, OID_LEN, Type, Value>;

/** @brief Get the ASN object type from the given templatized type.
 *         Specialize this template for handling a specific type.
 *  @tparam T - type of object from ASN type would be decided.
 *  @returns the ASN object type.
 */
template <typename T>
u_char getASNType() = delete;

template <>
u_char getASNType<uint32_t>()
{
   return ASN_UNSIGNED;
}

template <>
u_char getASNType<uint64_t>()
{
   return ASN_OPAQUE_U64;
}

template <>
u_char getASNType<int32_t>()
{
   return ASN_INTEGER;
}

template <>
u_char getASNType<std::string>()
{
   return ASN_OCTET_STR;
}

/** @class Notification
 *  @brief Notification interface.
 *
 *  This class implements the sendTrap function which
 *  send the list of objects defined by the specific notification
 *  to the configured SNMP manager.
 */

class Notification
{

    public:
        Notification() = default;
        Notification(const Notification&) = delete;
        Notification(Notification&&) = default;
        Notification& operator=(const Notification&) = delete;
        Notification& operator=(Notification&&) = default;
        virtual ~Notification() = default;

        /** @brief Send the snmp trap to the configured
         *          manager.
         */
        void sendTrap();

    protected:
        /** @brief Add the variable in the snmp pdu object.
         *  @param[in] pdu - SNMP pdu object.
         *  @param[in] objID -  SNMP object identifier.
         *  @param[in] objIDLen - Object identifier length.
         *  @param[in] type - ASN type of object.
         *  @param[in] val - Value of the object.
         *  @returns true on success otherwise false.
         */
        bool addPDUVar(netsnmp_pdu& pdu, const OID& objID, size_t objIDLen,
                       u_char type, Value val);

        /** @brief get the SNMP notification type in the mib
         *         defined format.
         *         This is pure virtual function all the subclasses
         *         need to provide its own defined type.
         *  @returns the notification type string.
         */
        virtual std::pair<OID, OID_LEN> getTrapOID() = 0;

        /** @brief get all the objects meta data defined under
         *         this notification.
         */
        virtual std::vector<Object> getFieldOIDList() = 0;

};

class TestErrorNotification;

/** @class ErrorNotification
 *  @brief subclass of Notification
 *
 *  A Error Notification represents the objects needed by the
 *  Error Object.
 */
class ErrorNotification : public Notification
{

    private:
        uint32_t errorID;
        uint64_t errorTimestamp;
        int32_t errorSeverity;
        std::string errorMessage;

    public:
        ErrorNotification() = default;
        ErrorNotification(const ErrorNotification&) = delete;
        ErrorNotification(ErrorNotification&&) = default;
        ErrorNotification& operator=(const ErrorNotification&) = delete;
        ErrorNotification& operator=(ErrorNotification &&) = default;
        ~ErrorNotification() = default;

        /** @brief Constructor
         *  @param[in] id - The error entry id.
         *  @param[in] ts - The commit timestamp.
         *  @param[in] sev - The severity of the error.
         *  @param[in] msg - The message of the error.
         */
        ErrorNotification(uint32_t id, uint64_t ts, int32_t sev,
                          std::string msg) :
            errorID(id), errorTimestamp(ts),
            errorSeverity(sev), errorMessage(msg) {}

    protected:

        std::pair<OID, OID_LEN> getTrapOID() override
        {
            // notification sub types
            OID id =  {1, 3, 6, 1, 4, 1, 49871, 0, 1};
            OID_LEN idLen = 9;
            return std::make_pair<OID, OID_LEN>(std::move(id), std::move(idLen));
        }

        std::vector<Object> getFieldOIDList() override
        {
            std::vector<Object> objectList;

            OID_LEN idLen = 9;
            OID id = {1, 3, 6, 1, 4, 1, 49871, 1, 1};
            auto  type = getASNType<decltype(errorID)>();
            auto tuple = std::tuple<OID, OID_LEN, Type, Value>(
                    id, idLen, type, errorID);

            objectList.emplace_back(std::move(tuple));

            id = {1, 3, 6, 1, 4, 1, 49871, 1, 2};
            type = getASNType<decltype(errorTimestamp)>();
            tuple = std::tuple<OID, OID_LEN, Type, Value>(
                    id, idLen, type, errorTimestamp);

            objectList.emplace_back(std::move(tuple));

            id = {1, 3, 6, 1, 4, 1, 49871, 1, 3};
            type = getASNType<decltype(errorSeverity)>();

            tuple = std::tuple<OID, OID_LEN, Type, Value>(
                    id, idLen, type, errorSeverity);

            objectList.emplace_back(std::move(tuple));

            id = {1, 3, 6, 1, 4, 1, 49871, 1, 4};
            type = getASNType<decltype(errorMessage)>();

            tuple = std::tuple<OID, OID_LEN, Type, Value>(
                    id, idLen, type, errorMessage);

            objectList.emplace_back(std::move(tuple));

            return objectList;
        }

        friend class TestErrorNotification;

};

} // namespace snmp
} // namespce network
} // namespce phosphor