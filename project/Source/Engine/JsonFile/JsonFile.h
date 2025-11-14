#include "nlohmann/json.hpp"
#include <fstream>
#include<unordered_map>

using Json = nlohmann::json;

Json LoadJson(const std::string& path);

class JsonFile {
public:
    enum Handle {
        TEST,
    };
    static Json GetJsonFiles(Handle handle) { return jsonFiles_[handle]; };
    static void LoadAllJsonFile();
private:
    static std::unordered_map<Handle, Json> jsonFiles_;

};