#pragma once
#include<memory>
#include"WorldTransform.h"
class Sprite;

class Shutter
{
public:
    void Initialize();
    void Create();
    void Draw();
    void Close(const float& timer);
    void Open(const float& timer);
private:
    void EasingTransformY(const float& start, const float& end, const float& timer);
private:
    std::unique_ptr<Sprite> shutterSprite_ = nullptr;
    WorldTransform2D shutterWorldTransform_;
    std::unique_ptr<Sprite> closeWordSprite_ = nullptr;
    WorldTransform2D closeWordWorldTransform_;
    Vector2 wordSize_ = { 640.0f,256.0f };
    bool isAnnounce_ = false;
};

