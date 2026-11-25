#pragma once
#include<memory>
#include<array>
#include"Vector2.h"

class Sprite;
class FirstStoryZero
{
private:

    struct AnimationSprite {
        std::unique_ptr<Sprite>sprite;
        Vector2 pos;
        float timer;
    };

    std::array<AnimationSprite, 3>aniSprites_;
    Vector2 handShakeStartPos_;
    Vector2 handShakeEndPos_;
public:
    enum Type {
        kCalendar,
        kGirl,
        kHand,
    };
    FirstStoryZero();
    ~FirstStoryZero();
    void Initialize();
    void Update();
    void Draw();
    void Debug();
};