#include "JsonFile.h"

std::unordered_map<std::string, JsonData>JsonFile::jsonFiles_;

void JsonFile::LoadAllJsonFile()
{
    LoadJson("config");
}

void JsonFile::LoadJson(const std::string& Tag) {

    if (jsonFiles_.contains(Tag)) {
        return;
    }

    jsonFiles_[Tag].path = "Resource/JsonFiles/" + Tag + ".json";
    std::ifstream file(jsonFiles_[Tag].path);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open JSON file: " + jsonFiles_[Tag].path);
    }

    file >> jsonFiles_[Tag].data;

}
void JsonFile::SaveJson(const std::string& Tag) {

    if (jsonFiles_.contains(Tag)) {
        std::ofstream file(jsonFiles_[Tag].path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open JSON file for writing: " + jsonFiles_[Tag].path);
        }
        file << jsonFiles_[Tag].data.dump(4); // インデント付きで保存（4スペース）
    } else {
        throw std::runtime_error("Tag is UnKnown");
    }

}
void JsonFile::SetJson(const std::string& tag, const nlohmann::json& j) {
    jsonFiles_[tag].data = j;
    jsonFiles_[tag].path = "Resource/JsonFiles/" + tag + ".json";
}