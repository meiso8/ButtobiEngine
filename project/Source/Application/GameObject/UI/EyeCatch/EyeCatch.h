#pragma once
#include<memory>
#include<array>
#include<stdint.h>
class Sprite;

class EyeCatch
{
public:
    EyeCatch();
    ~EyeCatch();
    void Initialize();
    void Update();
    void Draw();
private:
    std::array<std::unique_ptr<Sprite>,8>  sprites_;
    std::array<int32_t,8>textureHandles_;
    float animationTimer_ = 0.0f;
    uint32_t animeCount_ = 0;
    bool isAnimeEnd_ = false;
};

