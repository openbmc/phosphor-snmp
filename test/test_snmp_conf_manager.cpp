#include "snmp_conf_manager.hpp"
#include "xyz/openbmc_project/Common/error.hpp"

#include <sdbusplus/bus.hpp>

#include <gtest/gtest.h>

namespace phosphor
{
namespace network
{
namespace snmp
{

auto managerObjPath = "/xyz/openbmc_test/snmp/manager";
using InvalidArgument =
    sdbusplus::xyz::openbmc_project::Common::Error::InvalidArgument;

class TestSNMPConfManager : public testing::Test
{
  public:
    sdbusplus::bus_t bus;
    ConfManager manager;
    // confDir could have been created locally in the
    // TestSNMPConfManager but somehow that is leading
    // to segmentation fault while running the unit test.
    // TODO: https://github.com/openbmc/phosphor-snmp/issues/5
    std::string confDir;
    TestSNMPConfManager() :
        bus(sdbusplus::bus::new_default()), manager(bus, managerObjPath)
    {
        char tmp[] = "/tmp/snmpManager.XXXXXX";
        confDir = mkdtemp(tmp);
        manager.dbusPersistentLocation = confDir;
    }

    ~TestSNMPConfManager()
    {
        fs::remove_all(manager.dbusPersistentLocation);
    }

    std::string createSNMPClient(std::string ipaddress, uint16_t port)
    {
        return manager.client(ipaddress, port);
    }

    ClientList& getSNMPClients()
    {
        return manager.clients;
    }

    bool isClientExist(const std::string& ipaddress)
    {
        for (const auto& val : manager.clients)
        {
            if (val.second.get()->address() == ipaddress)
            {
                return true;
            }
        }
        return false;
    }

    void deleteSNMPClient(const std::string& ipaddress)
    {
        std::vector<size_t> ids{};
        for (const auto& val : manager.clients)
        {
            if (val.second.get()->address() == ipaddress)
            {
                ids.emplace_back(val.second.get()->getClientId());
            }
        }

        for (const auto& id : ids)
        {
            if (manager.clients.contains(id))
            {
                manager.clients.at(id)->delete_();
            }
        }
    }
};

// Add single SNMP client
TEST_F(TestSNMPConfManager, AddSNMPClient)
{
    // check the created object path
    auto path = createSNMPClient("192.168.1.1", 24);
    std::string expectedPath = managerObjPath;
    expectedPath += std::string("/1");

    EXPECT_EQ(path, expectedPath);

    // check whether the client created
    auto& clients = getSNMPClients();
    EXPECT_EQ(1U, clients.size());
    EXPECT_EQ(true, isClientExist("192.168.1.1"));
}

// Add multiple SNMP client
TEST_F(TestSNMPConfManager, AddMultipleSNMPClient)
{
    // add multiple clients and check whether the object path is generated
    // correctly.
    createSNMPClient("192.168.1.1", 24);
    auto path = createSNMPClient("192.168.1.2", 24);
    std::string expectedPath = managerObjPath;
    expectedPath += std::string("/2");

    EXPECT_EQ(path, expectedPath);

    // check both the clients get created
    auto& clients = getSNMPClients();
    EXPECT_EQ(2U, clients.size());

    EXPECT_EQ(true, isClientExist("192.168.1.1"));
    EXPECT_EQ(true, isClientExist("192.168.1.2"));
}

// Add duplicate SNMP client
TEST_F(TestSNMPConfManager, AddDuplicateSNMPClient)
{
    createSNMPClient("192.168.1.1", 24);
    EXPECT_THROW(createSNMPClient("192.168.1.1", 24), InvalidArgument);
}

// Delete SNMP client
TEST_F(TestSNMPConfManager, DeleteSNMPClient)
{
    createSNMPClient("192.168.1.1", 24);
    createSNMPClient("192.168.1.2", 24);
    createSNMPClient("192.168.1.1", 25);

    auto& clients = getSNMPClients();
    EXPECT_EQ(3U, clients.size());

    deleteSNMPClient("192.168.1.1");
    EXPECT_EQ(1U, clients.size());

    auto path = createSNMPClient("192.168.1.3", 24);
    std::string expectedPath = managerObjPath;
    expectedPath += std::string("/4");
    EXPECT_EQ(path, expectedPath);

    EXPECT_EQ(2U, clients.size());
    EXPECT_EQ(true, isClientExist("192.168.1.2"));
    EXPECT_EQ(false, isClientExist("192.168.1.1"));
    EXPECT_EQ(true, isClientExist("192.168.1.3"));
}

} // namespace snmp
} // namespace network
} // namespace phosphor
