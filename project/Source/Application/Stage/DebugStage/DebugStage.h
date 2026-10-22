#pragma once
#include"../Stage.h"

class Network;

class DebugStage :public Stage
{
private:
    std::unique_ptr<Network>network_ = nullptr;
public:
    DebugStage();
    ~DebugStage()override;
    void Initialize()override;
    void StageTransitionInitialize()override;
    void Update()override;
    void Draw()override;
    void CheckCollision(CollisionManager& collisionManager, RaySprite& raySprite)override;
    void DrawSprite()override;
};

