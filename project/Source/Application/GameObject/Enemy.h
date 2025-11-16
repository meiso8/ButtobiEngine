#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include"AABB.h"
#include"CharacterState.h"

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
    void Draw(Camera& camera,const LightMode& lightMode);
    void Update();
    Vector3 GetWorldPosition()const override;
    void OnCollision()override;
    void SetTarget(Vector3& target) { target_ = &target; };
private:
    float actionTimer_ = 0.0f;
    //目標地点
    Vector3* target_ = nullptr;

    //モデル
    Model* model_;
    //体の位置
    Object3d bodyPos_;
    //キャラクターの共通でもつ状態
    CharacterState characterState_;
    
    enum State {
        FIRST,
        SECOND,
        END,
    };

    enum PHASE {
        APPROACH,
        ATTACK,
        EXIT,
        MAX_PHASE
    };

    //メンバ関数ポインタテーブル
    static void(Enemy::* spFuncTable[])();
    PHASE phase_ = PHASE::APPROACH;
    Vector3 velocity_ = { 0.0f };
private:
    void Fireball();
    void FloorChangeAttack();
    void Tackle();
    void ShockWaveAttack();
    void Exit();
};

