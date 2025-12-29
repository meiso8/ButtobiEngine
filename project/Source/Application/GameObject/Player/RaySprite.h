#pragma once
#include<memory>
#include"Line.h"
#include"Sprite.h"

class RaySprite
{
private:
    std::unique_ptr<Sprite>sprite_ = nullptr;
    float tMin_ = 0.0f;
    float tMax_ = 1.0f;
    void Update();

public:

    RaySprite();
    void UpdateRay(const Ray ray) {
        ray_ = ray;
        Update();
    };

    void Draw();
    bool IntersectsAABB(const AABB& aabb);
    Ray ray_ = {};


};

