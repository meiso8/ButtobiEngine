#include "JsonFile.h"
#include <filesystem>
std::unordered_map<std::string, JsonData>JsonFile::jsonFiles_;

void JsonFile::LoadAllJsonFile()
{
    std::string folder = "Resource/JsonFiles/";
    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        if (entry.path().extension() == ".json") {
            std::string filename = entry.path().stem().string(); // 拡張子なしのファイル名
            LoadJson(filename);
        }
    }
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

JsonFile::~JsonFile()
{
    jsonFiles_.clear();
}

const std::unordered_map <std::string, JsonData >JsonFile::GetJsonData()
{
    return jsonFiles_;
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
    JsonData& data = jsonFiles_[tag];
    data.data = j;
    data.path = "Resource/JsonFiles/" + tag + ".json";
    SaveJson(tag);
}