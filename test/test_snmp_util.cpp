#include <gtest/gtest.h>
#include <netinet/in.h>
#include "snmp_util.hpp"

namespace phosphor
{
namespace network
{
namespace snmp
{

TEST(TestUtil, IpValidation)
{
    std::string ipaddress = "0.0.0.0";
    EXPECT_EQ(true, isValidIP(AF_INET, ipaddress));

    ipaddress = "9.3.185.83";
    EXPECT_EQ(true, isValidIP(AF_INET, ipaddress));

    ipaddress = "9.3.185.a";
    EXPECT_EQ(false, isValidIP(AF_INET, ipaddress));

    ipaddress = "9.3.a.83";
    EXPECT_EQ(false, isValidIP(AF_INET, ipaddress));

    ipaddress = "x.x.x.x";
    EXPECT_EQ(false, isValidIP(AF_INET, ipaddress));

    ipaddress = "0:0:0:0:0:0:0:0";
    EXPECT_EQ(true, isValidIP(AF_INET6, ipaddress));

    ipaddress = "1:0:0:0:0:0:0:8";
    EXPECT_EQ(true, isValidIP(AF_INET6, ipaddress));

    ipaddress = "1::8";
    EXPECT_EQ(true, isValidIP(AF_INET6, ipaddress));
    ipaddress = "0:0:0:0:0:FFFF:204.152.189.116";
    EXPECT_EQ(true, isValidIP(AF_INET6, ipaddress));

    ipaddress = "::ffff:204.152.189.116";
    EXPECT_EQ(true, isValidIP(AF_INET6, ipaddress));

    ipaddress = "a:0:0:0:0:FFFF:204.152.189.116";
    EXPECT_EQ(true, isValidIP(AF_INET6, ipaddress));

    ipaddress = "1::8";
    EXPECT_EQ(true, isValidIP(AF_INET6, ipaddress));
}

} // namespae snmp
} // namespce network
} // namespace phosphor
