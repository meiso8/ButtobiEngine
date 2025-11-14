#include "JsonFile.h"

Json LoadJson(const std::string& path) {

    std::ifstream file(path);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open JSON file: " + path);
    }

    Json j;

    file >> j;

    return j;
}

std::unordered_map<JsonFile::Handle, Json>JsonFile::jsonFiles_;

void JsonFile::LoadAllJsonFile()
{
    jsonFiles_[TEST] = LoadJson("Resource/JsonFiles/config.json");

}
