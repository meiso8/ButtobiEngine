
#pragma once
#include"Object3d.h"
#include <unordered_map>
#include"Collider.h"
#include<memory>
class Model;
class Camera;
enum LightMode;

class HouseCollider :public Collider {

public:
    HouseCollider();
    void SettingAABB(const AABB& aabb,Model* model);
    void SettingSphere(const float& radius, Model* model);
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    /// @brief 衝突時コールバック関数
    void OnCollision(Collider* collider)override;
    Vector3 GetWorldPosition() const override;
    Object3d pos_;
    bool isHit_ = false;
};

class House
{
public:

    enum Objects {
 /*       kHouse_floor,*/
        kHouse_wall_L,
        kHouse_wall_R,
        kHouse_wall_back_L,
        kHouse_wall_back_R,
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
    std::unordered_map<Objects, std::unique_ptr<HouseCollider>>& GetColliders() {
        return colliders_
            ;
    }

    bool isWallBrake_ = false;

private:
    int* hitCount_ = nullptr;
    int maxHitCount_ = 0;
    std::unordered_map<Objects, std::unique_ptr<HouseCollider>> colliders_;
    float wallBreakTimer_ = 0.0f;
    bool isDoorOpen_ = false;

    Object3d floorPos_;
    Object3d doorPos_;
    Object3d wallBackPos_;
};