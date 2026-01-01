#pragma once
#include"../Stage.h"

#include "PapyrusWall.h"
#include"Water.h"
#include<memory>

class WaterStage :public Stage
{
private:
    std::unique_ptr<Water>water_ = nullptr;
    std::unique_ptr<PapyrusWall>papyrusWall_ = nullptr;
public:
    WaterStage();
    void Initialize()override;
    void Update()override;
    void Draw(Camera& camera)override;
    void CheckCollision(CollisionManager& collisionManager)override;
};

