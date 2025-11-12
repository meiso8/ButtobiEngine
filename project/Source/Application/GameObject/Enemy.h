#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include"AABB.h"
#include"CharacterState.h"
#include"CubeMesh.h"
#include<memory>

class Model;
class Camera;

class Enemy
{
public:

    Enemy();
    void Init();
    void Draw(Camera& camera);
    void Update();
    AABB GetWorldAABB();
    void OnCollision();
    void SetTarget(Vector3& target) { target_ = &target; };
private:
    //目標地点
    Vector3* target_ = nullptr;
    //AABB
    AABB aabb_;
    //立体のメッシュデバック用
    std::unique_ptr<CubeMesh>cubeMesh_;
    //モデル
    Model* model_;
    //体の位置
    Object3d bodyPos_;
    //キャラクターの共通でもつ状態
    CharacterState characterState_;

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
    void Approach();
    void Attack();
    void Exit();
};

