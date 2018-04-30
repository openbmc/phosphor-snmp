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
    TestSNMPConfManager() : bus(sdbusplus::bus::new_default()), manager(bus, "")
    {
        char tmp[] = "/tmp/snmpManager.XXXXXX";
        std::string confDir = mkdtemp(tmp);
        manager.dbusPersistentLocation = confDir;
    }

    ~TestSNMPConfManager()
    {
        fs::remove_all(manager.dbusPersistentLocation);
    }

    void createSNMPClient(std::string ipaddress, uint16_t port)
    {
        manager.client(ipaddress, port);
    }

    ClientList &getSNMPClients()
    {
        return manager.clients;
    }

    void deleteSNMPClient(std::string ipaddress)
    {
        auto &it = manager.clients[ipaddress];
        it->delete_();
    }
};

// Add Single SNMP Client
TEST_F(TestSNMPConfManager, AddSNMPClient)
{
    using namespace sdbusplus::xyz::openbmc_project::Common::Error;

    createSNMPClient("192.168.1.1", 24);

    auto &clients = getSNMPClients();
    EXPECT_EQ(1, clients.size());
    EXPECT_EQ(true, clients.find("192.168.1.1") != clients.end());
}

// Add Multiple SNMP Client
TEST_F(TestSNMPConfManager, AddMultipleSNMPClient)
{
    using namespace sdbusplus::xyz::openbmc_project::Common::Error;

    createSNMPClient("192.168.1.1", 24);
    createSNMPClient("192.168.1.2", 24);

    auto &clients = getSNMPClients();
    EXPECT_EQ(2, clients.size());
    EXPECT_EQ(true, clients.find("192.168.1.1") != clients.end());
    EXPECT_EQ(true, clients.find("192.168.1.2") != clients.end());
}

// Delete SNMP Client
TEST_F(TestSNMPConfManager, DeleteSNMPClient)
{
    using namespace sdbusplus::xyz::openbmc_project::Common::Error;

    createSNMPClient("192.168.1.1", 24);
    createSNMPClient("192.168.1.2", 24);

    auto &clients = getSNMPClients();
    EXPECT_EQ(2, clients.size());
    deleteSNMPClient("192.168.1.1");
    EXPECT_EQ(1, clients.size());
    EXPECT_EQ(true, clients.find("192.168.1.2") != clients.end());
}

} // namespace snmp
} // namespce network
} // namespace phosphor
