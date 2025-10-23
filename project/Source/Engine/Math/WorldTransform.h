#pragma once

#include"Vector3.h"
#include"Matrix4x4.h"

class WorldTransform {
public://一旦public
    Vector3 scale_ = { 0.0f };
    Vector3 rotate_ = {0.0f};
    Vector3 translate_ = { 0.0f };
    Matrix4x4 matWorld_ = {0.0f};
    const WorldTransform* parent_ = nullptr;
public:
    void Initialize();
    Vector3 GetWorldPosition()const;
};

void WorldTransformUpdate(WorldTransform& worldTransform);
