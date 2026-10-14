#include "FlagManager.h"
#include"ImGui.h"

std::unordered_map < std::string, bool >FlagManager::flags_;

void FlagManager::SetFlag(std::string flagName, bool flag)
{
    flags_[flagName] = flag;
}

bool FlagManager::HasFlag(std::string flagName)
{
    return flags_.contains(flagName)&&flags_[flagName];
}

void FlagManager::InitializeFlags()
{
    for (auto& [name,flag]:flags_) {
        flag = false;
    }
}

void FlagManager::ClearFlags()
{
    flags_.clear();
}

void FlagManager::DebugFlags()
{
#ifdef _DEVELOP
    ImGui::Begin("FlagManager");

    for (auto& [name, flag] : flags_) {
        ImGui::Checkbox(name.c_str(), &flag);
    }
    ImGui::End();
#endif
}
