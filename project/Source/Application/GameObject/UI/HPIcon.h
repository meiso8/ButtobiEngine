#pragma once
#include<memory>
#include"Sprite.h"
#include<map>
#include<vector>
#include"CharacterState.h"
#include"Shake.h"
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
    void Setting(const Vector2& size, const Vector2& pos);
    void Initialize();
    void Update();
    void Draw();
    void HitAction();
    static const int kMaxHPIcon_ = 5;
    ~HPIcon();
private:
    HPs* hps_ = 0;
    int preHP;
    bool* isHitPtr_;
    std::map<SpriteTypes, std::vector<std::unique_ptr<Sprite>>> sprites_;
    std::vector<Vector2> position_;
    float timer_ = 0.0f;
    float maxTime_ = 1.0f;
    float theta_ = 0.0f;
    size_t drawHpNum_ = 0;
    std::unique_ptr<Shake>shake = nullptr;
};
