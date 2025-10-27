#pragma once
#include<memory>
class Sprite;
#include"WorldTransform.h"

class AppleSceneChange
{
private:
    std::unique_ptr<Sprite> appleSprite_ = nullptr;
    Vector2 size_ = { 1280.0f,720.0f };
    WorldTransform2D worldTransform_;
public :
    void Initialize();
    void Update(const float& timer);
    void Draw();
};

