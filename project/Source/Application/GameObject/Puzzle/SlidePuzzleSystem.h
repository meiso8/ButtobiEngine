#pragma once

#include"Player/RaySprite.h"
#include<memory>
#include"Puzzle.h"
#include"Camera.h"
#include"PuzzleObj.h"

class SlidePuzzleSystem {
public:
    SlidePuzzleSystem();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    void RayCastHit(RaySprite& ray);
    bool GetIsGameEnd() {
        //アクティブをfalseにする
        isActive_ = false;
        return clearTimer_ <= 0.0f;
    };
    static bool IsActive() { return isActive_; };
    static void SetActive(bool active) { isActive_ = active; };
private:
    std::unique_ptr<Puzzle>puzzle_ = nullptr;
    std::unique_ptr<PuzzleObj>puzzleObj_ = nullptr;

    const float maxTimer_ = 5.0f;
    float clearTimer_ = maxTimer_;
    static bool isActive_;
};
