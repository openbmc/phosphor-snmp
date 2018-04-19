#include "snmp_conf_manager.hpp"

#include "xyz/openbmc_project/Common/error.hpp"

#include <gtest/gtest.h>
#include <sdbusplus/bus.hpp>

namespace phosphor
{
namespace network
{
namespace snmp
{

class TestSNMPConfManager : public testing::Test
{
    public:

        sdbusplus::bus::bus bus;
        ConfManager manager;
        std::string confDir;
        TestSNMPConfManager()
            : bus(sdbusplus::bus::new_default()),
              manager(bus, "/tmp/test") {};

        ~TestSNMPConfManager()
        {
        }

        void createSNMPClient(std::string ipaddress, uint16_t port,
                              IPProtocol addressType)
        {
            manager.client(ipaddress, port, addressType);
        }

        ClientList& getSNMPClients()
        {
            return manager.clients;
        }

        void deleteSNMPClient(std::string ipaddress)
        {
             if (manager.clients.find(ipaddress) != manager.clients.end())
             {
                 auto& it = manager.clients[ipaddress];
                 it->delete_();
             }
        }

};

// Add Single SNMP Client
TEST_F(TestSNMPConfManager, AddSNMPClient)
{
    using namespace sdbusplus::xyz::openbmc_project::Common::Error;
    bool caughtException = false;
    try
    {
        createSNMPClient("192.168.1.1", 24, IPProtocol::IPv4);
    }
    catch (InternalFailure& e)
    {
        caughtException = true;
    }

    EXPECT_EQ(false, caughtException);
    auto& clients = getSNMPClients();
    EXPECT_EQ(1, clients.size());
    auto found = (clients.find("192.168.1.1") != clients.end()) ? true : false;
    EXPECT_EQ(true, found);
}

// Add Multiple SNMP Client
TEST_F(TestSNMPConfManager, AddMultipleSNMPClient)
{
    using namespace sdbusplus::xyz::openbmc_project::Common::Error;
    bool caughtException = false;
    try
    {
        createSNMPClient("192.168.1.1", 24, IPProtocol::IPv4);
        createSNMPClient("192.168.1.2", 24, IPProtocol::IPv4);
    }
    catch (InternalFailure& e)
    {
        caughtException = true;
    }

    EXPECT_EQ(false, caughtException);
    auto& clients = getSNMPClients();
    EXPECT_EQ(2, clients.size());
    auto found = (clients.find("192.168.1.1") != clients.end()) ? true : false;
    EXPECT_EQ(true, found);
    found = (clients.find("192.168.1.2") != clients.end()) ? true : false;
    EXPECT_EQ(true, found);
}

// Delete SNMP Client
TEST_F(TestSNMPConfManager, DeleteSNMPClient)
{
    using namespace sdbusplus::xyz::openbmc_project::Common::Error;
    bool caughtException = false;
    try
    {
        createSNMPClient("192.168.1.1", 24, IPProtocol::IPv4);
        createSNMPClient("192.168.1.2", 24, IPProtocol::IPv4);
    }
    catch (InternalFailure& e)
    {
        caughtException = true;
    }

    EXPECT_EQ(false, caughtException);
    auto& clients = getSNMPClients();
    EXPECT_EQ(2, clients.size());
    deleteSNMPClient("192.168.1.1");
    EXPECT_EQ(1, clients.size());
    auto found = (clients.find("192.168.1.2") != clients.end()) ? true : false;
    EXPECT_EQ(true, found);
}

}// namespace snmp
}// namespce network
}// namespace phosphor
