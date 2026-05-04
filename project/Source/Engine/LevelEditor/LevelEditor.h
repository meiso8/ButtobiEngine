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
    };

    struct PlayerSpawnData {
        EulerTransform transform;
    };
    struct EnemySpawnData {
        std::string fileName;
        EulerTransform transform;
    };

    std::vector<ObjectData>objects;
    //自キャラ配列
    std::vector<PlayerSpawnData>players;
    //敵キャラ配列
    std::vector<EnemySpawnData>enemies;
};

class LevelEditor
{
public:

    struct ObjectSet {
        std::unique_ptr<Object3d> obj_ = nullptr;
        std::unique_ptr<Collider>collider_ = nullptr;
    };

    LevelData* GetLevelData() { return levelData_.get(); };
    void Load(const std::string& fileName);
    /// @brief オブジェクトの作成関数
    /// @param objects 
    void CreateObject(std::vector<std::unique_ptr<ObjectSet>>&objects);
private:
    const std::string kDefaultBaseFirectory = "Resource/JsonFiles/";
    const std::string  kExtension = ".json";
    const uint32_t kMaxObjectCount_ = 1000;
    std::unique_ptr<LevelData>levelData_ = nullptr;
private:
    void LoadObject(nlohmann::json& object, LevelData* levelData);
    void LoadTransform(nlohmann::json& object, EulerTransform& transform);
};

