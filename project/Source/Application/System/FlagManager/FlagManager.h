#pragma once
#include<unordered_map>
#include<string>
class FlagManager
{
private:
   static std::unordered_map < std::string, bool >flags_;
public:
   static void SetFlag(std::string flagName, bool flag = true);
    static bool HasFlag(std::string flagName);
    static void InitializeFlags();
    static void ClearFlags();
    static void DebugFlags();
};

