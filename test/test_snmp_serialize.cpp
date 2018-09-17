#include <experimental/filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <netinet/in.h>

#include "snmp_client.hpp"
#include "snmp_conf_manager.hpp"
#include "snmp_serialize.hpp"

namespace phosphor
{
namespace network
{
namespace snmp
{

constexpr auto clientObjPath = "/xyz/openbmc_test/snmp/client";
namespace fs = std::experimental::filesystem;

class TestSerialize : public testing::Test
{
  public:
    sdbusplus::bus::bus bus;
    ConfManager manager;
    TestSerialize() :
        bus(sdbusplus::bus::new_default()),
        manager(bus, "/xyz/openbmc_test/snmp/manager")
    {
        char tmp[] = "/tmp/snmpManager.XXXXXX";
        std::string confDir = mkdtemp(tmp);
        manager.dbusPersistentLocation = confDir;
    }
    ~TestSerialize()
    {
        std::error_code ec;
        fs::remove_all(manager.dbusPersistentLocation, ec);
    }
};

TEST_F(TestSerialize, serialize)
{
    Client client(bus, clientObjPath, manager, "1.1.1.1", 23, IPProtocol::IPv4);

    auto path = serialize(client, manager.dbusPersistentLocation);
    Client restoreClient(bus, clientObjPath, manager);

    deserialize(path, restoreClient);

    EXPECT_EQ("1.1.1.1", restoreClient.address());
    EXPECT_EQ(23, restoreClient.port());
    EXPECT_EQ(IPProtocol::IPv4, restoreClient.addressFamily());
}

TEST_F(TestSerialize, deserialize_non_existent_file)
{
    Client client(bus, clientObjPath, manager);
    fs::path path = manager.dbusPersistentLocation;
    path /= "snmpTest";

    auto ret = deserialize(path, client);

    EXPECT_EQ(false, ret);
}

TEST_F(TestSerialize, deserialize_empty_file)
{
    Client restoreClient(bus, clientObjPath, manager);

    std::fstream file;

    fs::path path = manager.dbusPersistentLocation;
    path /= "snmpTest";

    file.open(path.string(), std::ofstream::out);
    file.close();
    // deserialize the object with empty file
    auto ret = deserialize(path, restoreClient);
    EXPECT_EQ(false, ret);
}

} // namespace snmp
} // namespace network
} // namespace phosphor
