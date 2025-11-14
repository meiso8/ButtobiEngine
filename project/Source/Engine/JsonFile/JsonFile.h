#include "nlohmann/json.hpp"
#include <fstream>
#include<unordered_map>

using Json = nlohmann::json;

struct JsonData {
    Json data;
    std::string path;
};

class JsonFile {
public:

    static Json& GetJsonFiles(const std::string& Tag) { return jsonFiles_[Tag].data; };
    static void SaveJson(const std::string& Tag);
    static void LoadJson(const std::string& Tag);
   static void LoadAllJsonFile();
    static void SetJson(const std::string& tag, const nlohmann::json& j);
private:
    static std::unordered_map <std::string, JsonData > jsonFiles_;

};