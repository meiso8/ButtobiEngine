#pragma once
#include<string>
#include<vector>
#include"Object3d.h"
#include"Collider.h"

struct LevelData {

    struct ObjectData {
        std::string fileName;
        std::unique_ptr<Object3d>obj;
        std::unique_ptr<Collider>collider;
    };

    std::vector<ObjectData>objects;
};

class LevelEditor
{
public:
    const std::string kDefaultBaseFirectory = "Resource/";
    const std::string  kExtension = ".json";
    void Load(const std::string& fileName);
};

