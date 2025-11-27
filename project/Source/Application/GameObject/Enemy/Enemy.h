#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include"AABB.h"
#include"CharacterState.h"
#include"Circle.h"
#include"Collider.h"
#include<memory>
#include<unordered_map>
class Model;
class Camera;
enum LightMode;

class Enemy :public Collider
{
public:
    enum Parts {
        BODY,
        WING_L,
        WING_R
    };

    Enemy();
    void Init();
    void Draw(Camera& camera, const LightMode& lightMode);
    void Update();
    //コライダーのオーバーライド
    Vector3 GetWorldPosition()const override;
    void OnCollision(Collider* collder)override;
    void SetTarget(Vector3& target) { target_ = &target; };
    void SetPlayerPos(Vector3& target) { playerPos_ = &target; };
    Vector3 GetPlayerTarget() {
        if (playerPos_ != nullptr) {
            return Normalize(*playerPos_ - bodyPos_.worldTransform_.GetWorldPosition());
        }
        return { 0.0f };
    }

    bool isShot_ = false;
    bool isBombShot_ = false;
    bool isWaveShot_ = false;
    bool isReqestClearFloor_ = false;
    //体の位置
    Object3d bodyPos_;

    bool IsHit() { return damageStruct_.isHit; }
    HPs* GetHpsPtr() { return &damageStruct_.hps; }
private:
    Model* model_ = nullptr;
    Object3d wingLPos_;
    Object3d wingRPos_;

    //目標地点
    Vector3* target_ = nullptr;
    Vector3* playerPos_ = nullptr;
    //キャラクターの共通でもつ状態
    Damage damageStruct_;
    Circle enemyRoundCircle_ = { {0.0f,0.0f,0.0f} ,7.0f };

    enum PHASE {
        TACKLE,
        FIREBALL,
        FLOORCHANGEATTACK,
        SHOCKWAVEATTACK,

        LERP_SQUARE_POS,
        LERP_ROUND_POS,
        KNOCKBACK,
        ROUND,
        SQUARE,
        RANDOM_WALK,

        EXIT,
        MAX_PHASE
    };

    //メンバ関数ポインタテーブル
    std::unordered_map<PHASE, std::function<void()>> UpdateActions_;
    PHASE phase_ = PHASE::ROUND;
    std::string currentState_ = "First";
    //メンバ関数ポインタテーブル
    std::unordered_map<std::string, std::function<void()>> SwitchRandomAttackPhase_;
    Vector3 velocity_ = { 0.0f };
    SphericalCoordinate sphericalPos_;
    Vector3 startPos_ = { 0.0f };
    Vector3 endPos_ = { 0.0f };

    float phaseTimer_ = 0.0f;
    float phaseTime_ = 0.0f;
    float poyoAnimTimer_ = 0.0f;
    const float kPoyoAnimeTime_ = 0.25f;
    //弾のクールタイム
    float fireBallCoolTimer_ = 0.0f;

    const float kLerpSquareTime_ = 2.0f;
    const float kSquareMoveSpeed_ = 0.05f;
    const float kSphericalLerpSpeed_ = 0.5f;
    float endRotateY_ = 0.0f;
    float startRotateY_ = 0.0f;
    float roundSpeedY = 1.0f;
    const float kRadius_ = 1.5f;
    const float kSize_ = 1.0f;
    int actionCount_ = 0;
    bool isAttack_ = false;
    bool isPreAttack_ = false;
    float wingTheta_ = 0.0f;
private:
    //状態を初期化
    void InitState();
    //状態を変更
    void SwitchState();
    //フェーズ変更
    void SetPhase(PHASE phase);
    void SwitchPhase();

    void RandomMovePhase();

    void RandomAttackPhaseFirst();
    void RandomAttackPhaseSecond();
    void RandomAttackPhaseEnd();


    void UpdatePhaseTimer();
    //回転移動
    void Round();
    void LerpRoundPos();

    void SquareMove();
    void LerpSquarePos();
    void RandomWalk();

    void LerpPos();

    //ファイアボール
    void Fireball();
    //床攻撃
    void FloorChangeAttack();
    //タックル
    void Tackle();
    void Knockback();
    //衝撃波攻撃
    void ShockWaveAttack();

    void Exit();

    void LookTarget(Vector3& target);
    void HitAnimation();
    void PoyoPoyo(const float& endTimer);
    void LerpScale();
    void RotateY(const float& timer);

    void Winging(const float& speed);
};

