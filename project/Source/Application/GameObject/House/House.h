
#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include <functional>
#include <unordered_map>
#include"Collider.h"

class Model;
class Camera;
enum LightMode;

class House
{
public:

    enum Objects {
        kHouse_floor,
        kHouse_wall_L,
        kHouse_wall_R,
        kHouse_wall_back,
        kHouse_door,
    };

    House();
    void Initialize();
    void Update();
    void Draw(Camera& camera, const LightMode& lightType);

    void SetHitCounts(int& hitCount, const int maxHitCount) {
        hitCount_ = &hitCount;
        maxHitCount_ = maxHitCount;
    }
private:
    int* hitCount_ = nullptr;
    int maxHitCount_ = 0;
    std::unordered_map<Objects, Object3d> positions_;
    float wallBreakTimer_ = 0.0f;
    bool isDoorOpen_ = false;
    bool isWallBrake_ = false;

};