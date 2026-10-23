#pragma once
#include"../Stage.h"

class DebugStage :public Stage
{
private:

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

