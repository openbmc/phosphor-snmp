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

#include <experimental/any>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace any_ns = std::experimental;

namespace phosphor
{
namespace snmp
{

using OID = std::array<oid,MAX_OID_LEN>;
using Type = char;
using Value = std::string;

//snmp trap ID
constexpr OID SNMPTrapOID = {1, 3, 6, 1, 6, 3, 1, 1, 4, 1, 0};

using Object = std::tuple<OID, Type, Value>;

/** @brief getObjectType Get the snmp object type
 *         from the given object.
 *  @param[in] var - Given object of any type.
 *  @returns the snmp object type.
 */

char getObjectType(any_ns::any& var)
{
    char type {};

    if (var.type() == typeid(uint64_t) ||
        var.type() == typeid(uint32_t) ||
        var.type() == typeid(uint16_t) ||
        var.type() == typeid(uint8_t))
    {
        type = 'u';
    }
    if (var.type() == typeid(int) ||
        var.type() == typeid(bool))
    {
        type = 'i';
    }
    else if (var.type() == typeid(uint64_t))
    {
        type = 'U';
    }
    else if (var.type() == typeid(std::string))
    {
        type = 's';
    }
    return type;
}

/** @class Notification
 *  @brief Notification interface.
 *
 *  Each subclass has to get its own notification type
 *  as well as the list of objects which is a part of
 *  that notification type.
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
        /** @brief get the SNMP notification type in the mib
         *         defined format.
         *         This is pure virtual function all the subclasses
         *         need to provide its own defined type.
         *  @returns the notification type string.
         */
        virtual std::string getNotificationType() = 0;

        /** @brief get all the objects meta data defined under
         *         this notification.
         */

        virtual std::vector<Object> getFieldOIDList() = 0;

};

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
    int errorSeverity;
    std::string errorMessage;
    int errorResolved;

public:
    ErrorNotification() = default;
    ErrorNotification(const ErrorNotification&) = delete;
    ErrorNotification(ErrorNotification&&) = default;
    ErrorNotification& operator=(const ErrorNotification&) = delete;
    ErrorNotification& operator=(Notification&&) = default;
    ~ErrorNotification() = default;


    ErrorNotification(uint32_t id, uint64_t ts, uint8_t sev, std::string msg,
                      bool reslv) :
        errorID(id), errorTimestamp(ts),
        errorSeverity(sev), errorMessage(msg),
        errorResolved(reslv){}

protected:

    std::string getNotificationType() override
    {
        return "OPENBMC-NOTIFICATION-MIB::errorNotification";
    }


    std::vector<Object> getFieldOIDList() override
    {
        std::vector<Object> objectList;
        OID id = {0};
        size_t idLen = MAX_OID_LEN;

        any_ns::any val = errorID;
        auto  type = getObjectType(val);

        std::stringstream ss {};
        ss << errorID;

        read_objid("OPENBMC-NOTIFICATION-MIB::errorID.0", id.data(), &idLen);

        objectList.emplace_back(std::make_tuple<OID, Type, Value>(
                                std::move(id),
                                std::move(type),
                                std::move(ss.str())));

        idLen = MAX_OID_LEN;
        id = {0};
        read_objid("OPENBMC-NOTIFICATION-MIB::errorTimestamp.0", id.data(), &idLen);

        val.clear();
        val = errorTimestamp;

        type = getObjectType(val);

        ss.str(std::string());

        ss << errorTimestamp;
        printf("Str=%s\n", ss.str().c_str());

        objectList.emplace_back(std::make_tuple<OID, Type, Value>(
                                std::move(id),
                                std::move(type),
                                std::move(ss.str())));

        idLen = MAX_OID_LEN;
        id = {0};

        val.clear();
        val = errorSeverity;

        type = getObjectType(val);
        ss.str(std::string());
        ss << errorSeverity;
        printf("Str=%s\n", ss.str().c_str());
        read_objid("OPENBMC-NOTIFICATION-MIB::errorSeverity.0", id.data(), &idLen);

        objectList.emplace_back(std::make_tuple<OID, Type, Value>(
                                std::move(id),
                                std::move(type),
                                std::move(ss.str())));

        idLen = MAX_OID_LEN;
        id = {0};

        val.clear();
        val = errorMessage;
        type = getObjectType(val);
        ss.str(std::string());
        ss << errorMessage;
        read_objid("OPENBMC-NOTIFICATION-MIB::errorMessage.0", id.data(), &idLen);

        objectList.emplace_back(std::make_tuple<OID, Type, Value>(
                                std::move(id),
                                std::move(type),
                                std::move(ss.str())));

        idLen = MAX_OID_LEN;
        id = {0};
        val.clear();
        val = errorResolved;
        type = getObjectType(val);
        ss.str(std::string());
        ss << errorResolved;
        read_objid("OPENBMC-NOTIFICATION-MIB::errorResolved.0", id.data(), &idLen);

        objectList.emplace_back(std::make_tuple<OID, Type, Value>(
                                std::move(id),
                                std::move(type),
                                std::move(ss.str())));

        return objectList;
    }

};


} // namespce snmp
} // namespce phosphor
