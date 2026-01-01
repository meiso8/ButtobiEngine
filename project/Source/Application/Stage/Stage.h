#pragma once
#include"Player/Player.h"
#include"CollisionManager.h"
#include"Item/ItemManager.h"

class Camera;
class Stage
{
protected:
    static Player* player_;
    static ItemManager* itemManager_;
public:
    Stage();
    static void SetItemManager(ItemManager* itemManager) {
        itemManager_ = itemManager;
    };
    static void SetPlayer(Player* player) { player_ = player; };
    virtual void CheckCollision(CollisionManager& collisionManager) = 0;
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw(Camera& camera) = 0;
};

