#include "config.h"

#include "snmp_serialize.hpp"

#include "snmp_client.hpp"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <phosphor-logging/lg2.hpp>

#include <fstream>

// Register class version
// From cereal documentation;
// "This macro should be placed at global scope"
CEREAL_CLASS_VERSION(phosphor::network::snmp::Client, CLASS_VERSION);

namespace phosphor
{
namespace network
{
namespace snmp
{

/** @brief Function required by Cereal to perform serialization.
 *  @tparam Archive - Cereal archive type (binary in our case).
 *  @param[in] archive - reference to Cereal archive.
 *  @param[in] manager - const reference to snmp manager info.
 *  @param[in] version - Class version that enables handling
 *                       a serialized data across code levels
 */
template <class Archive>
void save(Archive& archive, const Client& manager,
          const std::uint32_t /*version*/)
{
    archive(manager.address(), manager.port());
}

/** @brief Function required by Cereal to perform deserialization.
 *  @tparam Archive - Cereal archive type (binary in our case).
 *  @param[in] archive - reference to Cereal archive.
 *  @param[in] manager - reference to snmp manager info.
 *  @param[in] version - Class version that enables handling
 *                       a serialized data across code levels
 */
template <class Archive>
void load(Archive& archive, Client& manager, const std::uint32_t /*version*/)
{
    std::string ipaddress{};
    uint16_t port{};

    archive(ipaddress, port);

    manager.address(ipaddress);
    manager.port(port);
}

bool serialize(Id id, const Client& manager, const fs::path& dir)
{
    try
    {
        fs::create_directories(dir);

        fs::path fileName = dir;
        fileName /= std::to_string(id);

        std::ofstream os(fileName.string(), std::ios::binary);
        if (!os.is_open())
        {
            lg2::error("Failed to open file for serialization: {FILE}", "FILE",
                       fileName);
            return false;
        }

        cereal::BinaryOutputArchive oarchive(os);
        oarchive(manager);
        return true;
    }
    catch (const cereal::Exception& e)
    {
        lg2::error("Serialization failed: {ERROR}", "ERROR", e);
        return false;
    }
    catch (const fs::filesystem_error& e)
    {
        lg2::error("Filesystem error during serialization: {ERROR}", "ERROR",
                   e);
        return false;
    }
    catch (const std::exception& e)
    {
        lg2::error("Serialization failed: {ERROR}", "ERROR", e);
        return false;
    }
}

bool deserialize(const fs::path& path, Client& manager)
{
    try
    {
        if (fs::exists(path))
        {
            std::ifstream is(path.c_str(), std::ios::in | std::ios::binary);
            cereal::BinaryInputArchive iarchive(is);
            iarchive(manager);
            return true;
        }
        return false;
    }
    catch (const cereal::Exception& e)
    {
        lg2::error("Deserialization failed: {ERROR}", "ERROR", e);
        std::error_code ec;
        fs::remove(path, ec);
        return false;
    }
    catch (const fs::filesystem_error& e)
    {
        return false;
    }
}

} // namespace snmp
} // namespace network
} // namespace phosphor
