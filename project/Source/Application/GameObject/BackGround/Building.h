#pragma once

#include<memory>
#include"Object3d.h"
#include<unordered_map>
#include "CubeMesh.h"
class Camera;
class Model;

#include"AABB.h"

class Building
{
private:


    std::unique_ptr <Object3d> buildingPos;
    Model* model_ = nullptr;
public:

    enum AABBType {
        Wall0,
        Wall1,
        Wall2,
        Wall3,
        Floor,
    };

    Building();
    void Init();
    void Update();
    void Draw(Camera& camera);
    AABB GetWorldAABB(const AABBType& type);
    std::unordered_map<AABBType, std::unique_ptr < CubeMesh>>cubes_;
    std::unordered_map<AABBType, std::unique_ptr <Object3d>>wallPos_;
    std::unordered_map<AABBType, AABB>aabbs_;
};

