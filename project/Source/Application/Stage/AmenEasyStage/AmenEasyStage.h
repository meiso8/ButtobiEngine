#pragma once

#include"../Stage.h"

#include"Puzzle/SlidePuzzleSystem.h"

#include<memory>
#include"Memo/MemoManager.h"
#include"BackGround/BackGround.h"

class AmenEasyStage :public Stage
{

private:
    std::unique_ptr<SlidePuzzleSystem>slidePuzzleSystem_ = nullptr;
    std::unique_ptr<BackGround>backGround_ = nullptr;
    bool isGenerateItem_ = false;
public:

    AmenEasyStage();
    ~AmenEasyStage();
    void Initialize()override;
    void StageTransitionInitialize()override;
    void Update()override;
    void Draw()override;
    void DrawSprite()override;
    void CheckCollision(CollisionManager& collisionManager, RaySprite& raySprite)override;
};

