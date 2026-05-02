#pragma once
#include<string>
#include<vector>
#include "nlohmann/json.hpp"
#include"Transform.h"
#include<memory>
#include<fstream>
#include"Object3d/Object3d.h"
#include"../System/Collider.h"

struct LevelData {

    struct ColliderData {
        Vector3 center = {0.0f,0.0f,0.0f};
        Vector3 size = {1.0f,1.0f,1.0f};
    };

    struct ObjectData {
        std::string fileName;
        EulerTransform transform;
        ColliderData colliderData;
        bool disabled = false;
    };

    std::vector<ObjectData>objects;
};

class LevelEditor
{
public:

    struct ObjectSet {
        std::unique_ptr<Object3d> obj_ = nullptr;
        std::unique_ptr<Collider>collider_ = nullptr;
    };

    std::vector<std::unique_ptr<ObjectSet>>objects_;

    LevelData* GetLevelData() { return levelData_.get(); };
    void Load(const std::string& fileName);
    void CreateObject(std::vector<std::unique_ptr<ObjectSet>>&objects);
private:
    const std::string kDefaultBaseFirectory = "Resource/JsonFiles/";
    const std::string  kExtension = ".json";
    const uint32_t kMaxObjectCount_ = 1000;
    std::unique_ptr<LevelData>levelData_ = nullptr;
private:
    void LoadObject(nlohmann::json& object, LevelData* levelData);
};

