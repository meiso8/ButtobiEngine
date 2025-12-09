#pragma once
#include "HealItem.h"

class CollisionManager;

class HealItemSpawner {
public:
    static HealItemSpawner& Instance() {
        static HealItemSpawner instance;
        return instance;
    }

    void Initialize();
    void Update();
    void Draw(class Camera& camera);
    void SpawnHealItem(const Vector3& position);
    void AddCollider(CollisionManager* collisionManager);
    void Release();
    std::vector<std::unique_ptr<HealItem>>& GetHealItems() {
        return healItems_
            ;
    };
private:
    HealItemSpawner();
    ~HealItemSpawner();
    HealItemSpawner(const HealItemSpawner&) = delete;
    HealItemSpawner& operator=(const HealItemSpawner&) = delete;

    std::vector<std::unique_ptr<HealItem>> healItems_;
    const int kMaxHealItemCount_ = 10;
};