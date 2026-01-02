#pragma once
#include"../Stage.h"
#include"Mummy.h"
#include<memory>
#include"Player/RaySprite.h"
#include"Papyrus.h"
#include"Item/ItemManager.h"
#include"Memo/MemoManager.h"
#include "../Stage/WaterStage/PapyrusWall.h"
class MummyStage :public Stage
{
private:
    std::unique_ptr<Mummy>mummy_ = nullptr;
    std::unique_ptr<Papyrus> papyrus_ = nullptr;
    std::unique_ptr<PapyrusWall>papyrusWall_ = nullptr;
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

