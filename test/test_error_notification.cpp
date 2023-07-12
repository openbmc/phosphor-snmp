#include "snmp_notification.hpp"

#include <netinet/in.h>

#include <gtest/gtest.h>

namespace phosphor
{
namespace network
{
namespace snmp
{

constexpr size_t ERROR_NOTIF_FIELD_COUNT = 4;

class TestErrorNotification : public testing::Test
{
  public:
    OBMCErrorNotification notif;
    TestErrorNotification() : notif(0, 0, 0, "")
    {
        // Empty
    }
    std::vector<Object> getFieldOIDList()
    {
        return notif.getFieldOIDList();
    }
};

TEST_F(TestErrorNotification, VerifyErrorNotificationFields)
{
    auto oidList = getFieldOIDList();

    // verify the number of the fields in the notification.
    EXPECT_EQ(ERROR_NOTIF_FIELD_COUNT, oidList.size());

    // Verify the type of each field.
    EXPECT_EQ(ASN_UNSIGNED, oidList[0].type);

    EXPECT_EQ(ASN_OPAQUE_U64, oidList[1].type);
    EXPECT_EQ(ASN_INTEGER, oidList[2].type);
    EXPECT_EQ(ASN_OCTET_STR, oidList[3].type);
}

TEST_F(TestErrorNotification, GetASNType)
{
    auto type = getASNType<uint32_t>();
    EXPECT_EQ(ASN_UNSIGNED, type);

    type = getASNType<uint64_t>();
    EXPECT_EQ(ASN_OPAQUE_U64, type);

    type = getASNType<int32_t>();
    EXPECT_EQ(ASN_INTEGER, type);

    type = getASNType<std::string>();
    EXPECT_EQ(ASN_OCTET_STR, type);
}

} // namespace snmp
} // namespace network
} // namespace phosphor
