#pragma once
#include<memory>
#include"Line.h"
#include"Sprite.h"

class RaySprite
{
public:

    RaySprite();
    void UpdateRay(const Ray ray) { ray_ = ray; }
    void Draw();
    bool IntersectsAABB(const AABB& aabb);
    Ray ray_ = {};
private:
    std::unique_ptr<Sprite>sprite_ = nullptr;
    float tMin_ = 0.0f;
    float tMax_ = 1.0f;
};

