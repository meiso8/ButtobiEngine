#pragma once

#include<memory>
#include"Sprite.h"
#include<map>
#include"CharacterState.h"
#include"Shake.h"
class HPGage
{
public:

    enum SpriteTypes {
 
        MaxHp,
        layer2,
        layer1,
    };

    HPGage();
    void SetHpPtr(HPs* hp) { hps_ = hp; };
    void Setting(const Vector2& size, const Vector2& pos, const Vector2& anchorPoint);
    void Initialize();
    void Update();
    void Draw();
    void UpdateHitAction();
    bool* isHitPtr_ = nullptr;
    bool* isKnockBackPtr_ = nullptr;
private:
   std::unique_ptr<Shake> shake_ = nullptr;
    HPs* hps_ = 0;
    std::map<SpriteTypes, std::unique_ptr<Sprite>>sprites_;
    std::map<SpriteTypes,Vector2>pos_;
    int preHP_ = 0;
    float timer_  = 0.0f;
    float preScale_ =0.0f;
};

