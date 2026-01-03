#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include"AABB.h"
#include"CharacterState.h"
#include"Circle.h"
#include"Collider.h"
#include<memory>

#include"AnimationObject3d.h"
#include"Model/SkinningModel.h"
class Model;
class Camera;
enum LightMode;

class Enemy :public Collider
{
private:
    //キャラクターの共通でもつ状態
    CharacterState characterState_;

public:

    Enemy();
    void Init();
    void Draw(Camera& camera, const LightMode& lightMode);
    void Update();
    Vector3 GetWorldPosition()const override;
    void SetIsApper(const bool& flag) { isApper_ = flag; }
    const bool& GetIsApper() { return isApper_; }
    Vector3 GetWorldPos()const
    {
        return bodyPos_.worldTransform_.GetWorldPosition();
    }

    void OnCollision(Collider* collder)override;

    void SetTarget(Vector3& target) { target_ = &target; };
    Vector3 GetToTarget() { 
        if (target_ != nullptr) {
            return Normalize(*target_ - bodyPos_.worldTransform_.GetWorldPosition());
        }
        
        return { 0.0f };
    }
    bool isShotStart_ = false;
    //体の位置
    AnimationObject3d bodyPos_;
    HPs* GetHpsPtr() { return &characterState_.hps; }
    const bool& GetIsDead() { return characterState_.isDead; }
private:
   const float kScale_ = 5.0f;
    bool isApper_ = false;
    float timer_ = 0.0f;
    float actionTime_ = 0.0f;
    float attackTime_ = 10.0f;
    //目標地点
    Vector3* target_ = nullptr;

    //モデル
    Model* model_;
    //モデル
    Model* dancingModel_;
    //モデル
    Model* moveModel_;
    std::unique_ptr<SkinningModel>skinningModel_ = nullptr;

    Circle enemyRoundCircle_ = { {0.0f,0.0f,0.0f} ,10.0f};
    Circle enemyFieldCircle_ = { {0.0f,0.0f,0.0f} ,9.0f };

    enum PHASE {
        APPER,
        ROUND,
        FIREBALL,
        EXIT,
        MAX_PHASE
    };

    //メンバ関数ポインタテーブル
    std::unordered_map<PHASE, std::function<void()>> UpdateActions_;
    PHASE phase_ = PHASE::APPER;
    Vector3 velocity_ = { 0.0f };

    Vector3 startPos_ = { 0.0f };
    Vector3 endPos_ = { 0.0f };
    float poyoAnimTimer_ = 0.0f;
    float endRotateY_ = 0.0f;
    float startRotateY_ = 0.0f;
    float roundSpeedY = 1.0f;
private:
    void SetPhase(PHASE phase);
    void Apper();
    void Round();
    void Fireball();
    void Exit();

    void UpdateTimer();
    void Look();
    void PoyoPoyo(const float& endTimer = 0.25f);
    void HitUpdate();
    void LerpScale();
    void RotateY (const float& timer);
};

