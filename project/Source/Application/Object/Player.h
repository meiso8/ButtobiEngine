#pragma once
#include<array>
#include<memory>

#include"Vector4.h"
#include"WorldTransform.h"

struct AABB;
struct Sphere;
struct Plane;
struct OBB;

class FlashParticle;
class Enemy;
class Camera;
class Model;
class QuadMesh;
class AABBRenderer;

class SphereRenderer;

/// @brief 自キャラ
class Player {

public:
    enum AttackPhase {
        kNone,
        kCharge,
        kFire,
        kEnd,
        kTimeUp,
        kCloseShutter,
        kPhaseMax,
    };

    enum Parts {
        kHead,
        kBody,
        kLeftArm,
        kRightArm,
        kLeftLeg,
        kRightLeg,
        kNumParts,
    };

    // チャージ最大時間
    static inline constexpr float kMaxChargeTime = 7500.0f;
    bool isHitOBB_ = false;
        

public:
    /// @brief コンストラクタ
    Player();

    /// @brief デストラクタ
    ~Player();

    /// @brief 初期化
    /// @param camera カメラ
    /// @param position 位置
    void Initialize(Camera& camera, const Vector3& position);

    /// @brief 更新
    void Update();

    /// @brief 移動入力
    void InputMove();

    // @brief 攻撃入力
    void InputAttack();

    // @brief 攻撃アニメーション
    void AttackAnimation();
    //シャッターを閉めるアニメーション
    void ShutterCloseAnimation(const float time);

    /// @brief 描画
    /// @param camera カメラ
    void Draw(Camera& camera);

    /// @brief ワールド変換データを取得
    /// @return ワールド変換データ
    const WorldTransform& GetWorldTransform() const { return worldTransform_; };

    /// @brief 速度を取得
    /// @return 速度
    const Vector3& GetVelocity() const { return velocity_; };

    /// @brief 前方向を取得
    /// @return 前方向
    Vector3 GetForward() const;

    /// @brief ワールド座標を取得
    /// @return ワールド座標
    Vector3 GetWorldPosition() const;


    /// @brief 球の取得
    /// @return 球
    Sphere GetSphere() const;

    /// @brief HPの当たり判定
    /// @return 球
    Sphere GetHPSphere() const;


    /// @brief 敵との当たり判定
    /// @param enemy 敵
    void OnCollision(const Enemy* enemy);

    /// @brief 敵との当たり判定
    /// @param enemy 敵
    void OnCollisionHP(const Enemy* enemy);


    /// @brief 平面との当たり判定
    /// @param plane 平面
    void OnCollision(const Plane& plane);

    /// @brief OBBとの当たり判定
    /// @param obb OBB
    void OnCollision(const OBB& obb);

    bool IsAttack()const;
    bool IsCharge()const;

    void InitializeLife(int SetMaxLife) { life_ = SetMaxLife; };

    int GetLife() const { return life_; };

    /// @brief キック力を取得
    /// @return キック力
    Vector3 GetKickForce() const { return kickForce_; };
    void InitKickForce() { kickForce_ = { 0.0f }; }
    Vector3 GetAttackArea();
    float GetChargeTimer()const { return chargeTimer_; }
    bool GetIsInvincible() const{ return isInvincible_; }
    void SetParticle(FlashParticle* particle) { flashParticle_ = particle; }
    //ImGUi用
    void Debug();

    void SetPhase(AttackPhase attackPhase) { attackPhase_ = attackPhase;}
private:
    Camera* camera_ = nullptr;
    // ワールド変換データ
    WorldTransform worldTransform_;

    WorldTransform PartsWorldTransform_[Parts::kNumParts];
    WorldTransform DrawPartsWorldTransform_[Parts::kNumParts];
	WorldTransform shadowWorldTransform_;

    // モデル
    std::array<Model*, Parts::kNumParts> model_ = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	std::unique_ptr<QuadMesh> shadowModel_ = {nullptr};

    
    // 速度
    Vector3 velocity_ = {};
    // 加速度
    static inline const float kMoveAmount = 0.8f;
    // 重力加速度（下方向）
    static inline const float kGravityAcceleration = 9.8f;
    // キャラクターキックｄの当たり判定サイズ
    static inline const float kWidth = 4.0f;
    static inline const float kHeight = 4.0f;
    // キャラクターHPの当たり判定サイズ
    static inline const float kRadius = 2.0f;

    AttackPhase attackPhase_ = AttackPhase::kNone;

    Vector3 targetPartsScale_[kPhaseMax][Parts::kNumParts];
    Vector3 targetPartsRotate_[kPhaseMax][Parts::kNumParts];
    Vector3 targetPartsTranslate_[kPhaseMax][Parts::kNumParts];

    Vector3 defaultPartsOffset_[Parts::kNumParts];

    float kInterVal_ = 0.5f; // 補間速度

    float attackTimer_ = 0.0f;
    float attackTimerMax_ = 0.5f;

    bool isEndAninationEnd_ = false;
    float endAninationTimer_ = 0;
    float endAninationTimerMax_ = 0.2f;

    bool isAttack_ = false;

    int life_;

    bool isInvincible_ = false;
    float invincibleTimer_ = 0.0f;
    float invincibleTimerMax_ = 1.0f;

    // チャージ時間
    float chargeTimer_ = 1000.0f;
    // キック力
    Vector3 kickForce_ = { 0.0f,0.0f,0.0f };

    //色　変更しましたyoshida
    Vector4 objectColor_ = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 shadowObjectColor_ = {1.0f, 1.0f, 1.0f, 0.5f};

    // AABBのデバッグ描画
    //std::unique_ptr<AABBRenderer> aabbRenderer_ = nullptr;

    FlashParticle* flashParticle_ = nullptr;
    
#ifdef _DEBUG
    // 球のデバッグ描画
    std::unique_ptr<SphereRenderer> sphereRenderer_ = nullptr;

#endif // _DEBUG

    void ResetAttack();
};