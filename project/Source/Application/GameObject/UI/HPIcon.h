#pragma once
#include<memory>
#include"Sprite.h"
#include<map>
#include<vector>
#include"CharacterState.h"

class HPIcon
{
public:

    enum SpriteTypes {
        MaxHp,
        layer1,
    };

    HPIcon();
    void SetHpPtr(HPs* hp) { hps_ = hp; };
    void Setting(const Vector2& size, const Vector2& pos);
    void Initialize();
    void Update();
    void Draw();
    static const int kMaxHPIcon_ = 5;
private:
    HPs* hps_ = 0;
    int preHP;
    std::map<SpriteTypes, std::vector<std::unique_ptr<Sprite>>> sprites_;
    float timer_ = 0.0f;
    float maxTime_ = 1.0f;
    float theta_ = 0.0f;
    size_t drawHpNum_ = 0;
};
