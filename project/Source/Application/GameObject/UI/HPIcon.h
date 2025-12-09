#pragma once
#include<memory>
#include"Sprite.h"
#include<map>
#include<vector>
#include"CharacterState.h"
#include"Shake.h"
#include"Texture.h"
class HPIcon
{
public:

    enum SpriteTypes {
        MaxHp,
        layer1,
    };

    HPIcon();
    void SetHpPtr(HPs* hp) { hps_ = hp; };
    void SetIsHitPtr(bool& isHit) { isHitPtr_ = &isHit; };
    void SetMaxIconNum(const int& num) { maxIcon_ = num; }
    void Setting(const Vector2& size, const Vector2& pos);
    void Initialize();
    void Update();
    void Draw();
    void HitAction();
    void SetTextureHandle(Texture::TEXTURE_HANDLE handle) {
        textureHandle_ = handle;
    };
    ~HPIcon();


private:
    HPs* hps_ = 0;
    int preHP;
    bool* isHitPtr_;
    std::map<SpriteTypes, std::vector<std::unique_ptr<Sprite>>> sprites_;
    std::vector<Vector2> position_;
    float timer_ = 0.0f;
    float maxTime_ = 0.8f;
    float theta_ = 0.0f;
    size_t drawHpNum_ = 0;
    int maxIcon_ = 3;
    Texture::TEXTURE_HANDLE textureHandle_ = Texture::TEXTURE_HANDLE::WHITE_1X1;
    std::unique_ptr<Shake>shake = nullptr;
};
