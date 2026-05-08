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

const sdbusplus::vtable_t CommunityManager::vtable[] = {
    sdbusplus::vtable::start(),
    sdbusplus::vtable::property(
        "CommunityString", "s", CommunityManager::propertyGet,
        CommunityManager::propertySet,
        sdbusplus::vtable::property_::emits_change),
    sdbusplus::vtable::end(),
};

int CommunityManager::propertyGet(
    sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
    const char* /*property*/, sd_bus_message* reply, void* userdata,
    sd_bus_error* /*error*/)
{
    auto* self = static_cast<CommunityManager*>(userdata);
    return sd_bus_message_append(reply, "s", self->communityString.c_str());
}

int CommunityManager::propertySet(
    sd_bus* /*bus*/, const char* /*path*/, const char* /*interface*/,
    const char* /*property*/, sd_bus_message* value, void* userdata,
    sd_bus_error* /*error*/)
{
    auto* self = static_cast<CommunityManager*>(userdata);
    const char* str = nullptr;
    int r = sd_bus_message_read(value, "s", &str);
    if (r < 0)
    {
        return r;
    }
    self->communityString = str;
    self->persist();
    return 0;
}

CommunityManager::CommunityManager(sdbusplus::bus_t& bus, const char* objPath) :
    objectPath(objPath),
    iface(bus, objPath, communityStrInterface, vtable, this)
{
    // Restore persisted community string
    restore();
}

std::string CommunityManager::getCommunityString() const
{
    return communityString;
}

void CommunityManager::setCommunityString(const std::string& value)
{
    communityString = value;
    persist();
    iface.property_changed("CommunityString");
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
        file << communityString;
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
            std::getline(file, communityString);
            file.close();
            lg2::info("Restored community string from persistent storage");
        }
    }
}

} // namespace snmp
} // namespace network
} // namespace phosphor
