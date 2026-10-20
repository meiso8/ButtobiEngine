#pragma once

#include"RaySprite/RaySprite.h"
#include<memory>
#include"Puzzle.h"
#include"Camera.h"
#include"PuzzleObj.h"
#include"TextureFactory.h"
class SlidePuzzleSystem {
public:
    SlidePuzzleSystem(const int horizontal, const int vertical, TextureFactory::Handle handle);
    ~SlidePuzzleSystem();
    void Initialize(const Vector3& pos, const float& rotY);
    static void InitStaticActiveFlag();
    void SetCurrentPtr();
    void Update(const Vector2& screenPos);
    void Draw();
    void DrawUI();
    void RayCastHit(RaySprite& ray);
    bool GetIsGameEnd() {
        return isEnd_;
    };

    PuzzleObj* GetPuzzleObj() {
        return puzzleObj_.get();
    };

    static Puzzle* GetCurrentPuzzle() { return currentPuzzle_; };

private:
    static Puzzle* currentPuzzle_;
    std::unique_ptr<Puzzle>puzzle_ = nullptr;
    std::unique_ptr<PuzzleObj>puzzleObj_ = nullptr;
    bool isEnd_ = false;
    bool isActive_ = false;
};
