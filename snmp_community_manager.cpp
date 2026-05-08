#include "snmp_community_manager.hpp"

#include <phosphor-logging/lg2.hpp>

#include <filesystem>
#include <fstream>

namespace phosphor
{
namespace network
{
namespace snmp
{

namespace fs = std::filesystem;

CommunityManager::CommunityManager(sdbusplus::bus_t& bus,
                                   const char* objPath) :
    CommunityIface(bus, objPath, CommunityIface::action::defer_emit)
{
    restore();
    emit_object_added();
}

std::string CommunityManager::communityString(std::string value)
{
    if (value == CommunityIface::communityString())
    {
        return value;
    }

    auto str = CommunityIface::communityString(value);
    persist();
    return str;
}

void CommunityManager::persist() const
{
    fs::path dir(communityStrPersistPath);
    if (!fs::exists(dir))
    {
        fs::create_directories(dir);
    }

    fs::path filePath = dir / "community_string";
    std::ofstream file(filePath, std::ios::out | std::ios::trunc);
    if (file.is_open())
    {
        file << CommunityIface::communityString();
        file.close();
    }
    else
    {
        lg2::error("Unable to persist community string to {PATH}", "PATH",
                   filePath.string());
    }
}

void CommunityManager::restore()
{
    fs::path filePath = fs::path(communityStrPersistPath) / "community_string";

    if (fs::exists(filePath))
    {
        std::ifstream file(filePath);
        if (file.is_open())
        {
            std::string value;
            std::getline(file, value);
            file.close();
            CommunityIface::communityString(value);
            lg2::info("Restored community string from persistent storage");
        }
    }
}

} // namespace snmp
} // namespace network
} // namespace phosphor
