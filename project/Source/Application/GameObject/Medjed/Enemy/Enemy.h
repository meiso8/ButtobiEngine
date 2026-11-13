#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include"AABB.h"
#include"CharacterState.h"
#include"Circle.h"
#include"Collider.h"
#include<memory>

#include"AnimationObject3d.h"
#include"Sound.h"
#include"IEnemy/IEnemy.h"

class Model;
class Camera;

class Enemy :public Collider
{

public:

    struct ColliderGroup {
        Matrix4x4 matrix_;
        std::unique_ptr<Collider> collider_ = nullptr;
    };

public:

    //コンストラクタ
    Enemy();
    ~Enemy();
    //初期化
    void Init();
    //描画
    void Draw(const Object3d::LightMode& lightMode);
    //更新
    void Update();
    /// @brief 衝突コールバック
    /// @param collider 
    void OnCollision(Collider* collider)override;

    // =======================//フラグ//===================================
    void SetIsApper(const bool& flag) { isAppear_ = flag; }
    const bool& GetIsApper()const { return isAppear_; }

    //ショット
    bool GetIsShotStart() const { return isShotStart_; }
    void SetIsShotStart(const bool flag) { isShotStart_ = flag; }

    //状態
    const HPs* GetHpsPtr() const { return &characterState_.hps; }
    const bool& GetIsDead() const { return characterState_.isDead; }

    // =======================//トランスフォーム//===================================

    Vector3 GetWorldPos() const { return  bodyPos_->GetWorldTransform().GetWorldPosition(); }
    const WorldTransform& GetWorldTransform()const { return bodyPos_->GetWorldTransform(); }
    void SetScale(const Vector3& scale) { bodyPos_->SetScale(scale); }
    const Vector3 GetInitScale();

    void SetTranslate(const Vector3& translate) { bodyPos_->SetTranslate(translate); };
    const Vector3& GetTranslate() { return bodyPos_->GetTranslate(); };

    //マスク
    void SetMaskVol(const float volume) { bodyPos_->SetMaskVol(volume); }

    // =======================//行列//===================================
    //目のマトリックスの取得
    const std::unordered_map < std::string, Matrix4x4>& GetEyeMats()const { return eyeMats_; };
    // =======================//コライダー//===================================
    //コライダーグループの取得
    const std::unordered_map <std::string, ColliderGroup>& GetColliderGroup() const { return colliders_; };

    //=====================//ターゲット//=========================

    //ターゲットの設定
    void SetTarget(const Vector3* target) { target_ = target; };

    //ターゲットに向かうベクトル
    Vector3 GetToTarget()const;
    const Vector3* GetTarget() { return target_; };

    void Look();
    void SetAnimation(const std::string& animation) { bodyPos_->SetAnimation(animation); }
    const std::string GetCurrentAnimation() { return bodyPos_->GetCurrentAnimation(); };
    /// @brief 足音
    /// @param tag 
    void SoundFootStep(const SoundFactory::TAG tag);

    void RotateY(const float& startRotateY, const float& endRotateY, const float& timer);
    void InitPoyoTimer() { poyoAnimTimer_ = 0.0f; }
    bool PoyoPoyoUpdateAndGetEnd(const float& endTimer = 0.25f);

    void ChangeState(const IEnemy::STATE& state);
    const IEnemy::STATE GetState()const { return currentState_->GetState(); }
private:

    void HitUpdate();
    void LerpScale();

private:
    //モデル
    Model* model_ = nullptr;
    //体の位置
    std::unique_ptr<AnimationObject3d>bodyPos_ = nullptr;

    //キャラクターの共通でもつ状態
    CharacterState characterState_;
    //フラグ
    bool isShotStart_ = false;
    bool isAppear_ = false;

    //目標地点
    const Vector3* target_ = nullptr;

    //コライダーのマップ
    std::unordered_map <std::string, ColliderGroup>colliders_;
    //目の行列
    std::unordered_map < std::string, Matrix4x4>eyeMats_;

    float poyoAnimTimer_ = 0.0f;
    //ステート
    std::unordered_map<IEnemy::STATE,std::unique_ptr<IEnemy>>states_;
    IEnemy* currentState_ = nullptr;
};

