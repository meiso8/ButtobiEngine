#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include"AABB.h"
#include"CharacterState.h"
#include"Circle.h"
#include"Collider.h"
#include<memory>

class Model;
class Camera;
enum LightMode;

class Enemy :public Collider
{
public:

    Enemy();
    void Init();
    void Draw(Camera& camera, const LightMode& lightMode);
    void Update();
    Vector3 GetWorldPosition()const override;
    void OnCollision(Collider* collder)override;
    void SetTarget(Vector3& target) { target_ = &target; };
    Vector3 GetToTarget() { 
        if (target_ != nullptr) {
            return Normalize(*target_ - bodyPos_.worldTransform_.GetWorldPosition());
        }
        
        return { 0.0f };
    }
    bool isShot_ = false;
    //体の位置
    Object3d bodyPos_;
private:
    float timer_ = 0.0f;
    float actionTime_ = 0.0f;
    float fireBallCoolTime_ = 0.0f;
    //目標地点
    Vector3* target_ = nullptr;

    //モデル
    Model* model_;

    //キャラクターの共通でもつ状態
    CharacterState characterState_;

    Circle enemyRoundCircle_ = { {0.0f,0.0f,0.0f} ,7.0f};
    Circle enemyFieldCircle_ = { {0.0f,0.0f,0.0f} ,9.0f };

    enum State {
        FIRST,
        SECOND,
        END,
    };

    enum PHASE {
        ROUND,
        FIREBALL,
        FLOORCHANGEATTACK,
        TACKLE,
        KNOCKBACK,
        SHOCKWAVEATTACK,
        EXIT,
        MAX_PHASE
    };

    //メンバ関数ポインタテーブル
    std::unordered_map<PHASE, std::function<void()>> UpdateActions_;
    PHASE phase_ = PHASE::ROUND;
    Vector3 velocity_ = { 0.0f };
    SphericalCoordinate sphericalPos_;
    Vector3 startPos_ = { 0.0f };
    Vector3 endPos_ = { 0.0f };
    float poyoAnimTimer_ = 0.0f;
    float endRotateY_ = 0.0f;
    float startRotateY_ = 0.0f;

private:
    void SetPhase(PHASE phase);
    void Round();
    void Fireball();
    void FloorChangeAttack();
    void Tackle();
    void Knockback();
    void ShockWaveAttack();
    void Exit();
    void UpdateTimer();
    void LookTarget();
    void PoyoPoyo(const float& endTimer = 0.25f);
    void HitUpdate();
    void LerpScale();
    void RotateY (const float& timer);
};

