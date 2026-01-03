#pragma once
#include"../Stage.h"
#include"Mummy.h"
#include<memory>
#include"Player/RaySprite.h"
#include"Papyrus.h"
#include"Item/ItemManager.h"
#include"Memo/MemoManager.h"
#include "MummyRoom.h" // ← 追加

class MummyStage :public Stage
{
private:
    std::unique_ptr<Mummy>mummy_ = nullptr;
    std::unique_ptr<Papyrus> papyrus_ = nullptr;
    std::unique_ptr<MummyRoom> mummyRoom_ = nullptr; // ← 追加
public:
    const float maxTime_ = 5.0f;
    float medjedApperTime_ = maxTime_;
    void TimerUpdate();
    bool IsEndTime() { return medjedApperTime_ <= 0.0f; };
    MummyStage();
    void Initialize()override;
    void Update()override;
    void Draw(Camera& camera)override;

    bool IsRayCastHit(RaySprite& raysprite);
    void CheckCollision(CollisionManager& collisionManager)override;
    Mummy* GetMummy() {
        return mummy_
            .get();
    };

};

