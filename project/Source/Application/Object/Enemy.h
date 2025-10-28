#pragma once
#include "WorldTransform.h"
#include "Vector4.h"
#include "Collider.h"
#include <memory>
#include <string>
#include<cstdint>

struct OBB;
struct AABB;
struct Sphere;
struct Plane;
class Player;
class Model;
class Camera;
class RigidBody;
class AABBRenderer;
class SphereRenderer;
class AppleCrashParticle;
class FlashParticle;

/// @brief 敵
class Enemy : public Collider {
public:
    /// @brief コンストラクタ
    Enemy();

    /// @brief デフォルトデストラクタ
    ~Enemy();

    /// @brief 初期化
    /// @param position 位置
    void Initialize(const Vector3& position);

    /// @brief 更新
    void Update();

    /// @brief 描画
    /// @param camera カメラ
    void Draw(Camera& camera);

    /// @brief AABBの取得
    /// @return AABB
    AABB GetAABB() const;

    /// @brief 球の取得
    /// @return 球
    Sphere GetSphere() const;

    /// @brief ワールド座標を取得
    /// @return ワールド座標
    Vector3 GetWorldPosition() const override;

    /// @brief プレイヤーとの当たり判定
    /// @param player プレイヤー
    void OnCollision(Player* player);

    /// @brief 平面との当たり判定
    /// @param plane 平面
    void OnCollision(const Plane& plane);

    /// @brief OBBとの当たり判定
    /// @param obb OBB
    void OnCollision(const OBB& obb);

    /// @brief 衝突応答
    void OnCollision() override;

    /// @brief 位置の加算
    /// @param translate 加算する位置
    void AddTranslate(const Vector3& translate) { worldTransform_.translate_ += translate; };

    /// @brief 剛体の取得
    /// @return 剛体
    RigidBody* GetRigidBody() { return rigidBody_.get(); };

    /// @brief 速度の取得
    /// @return 速度
    Vector3 GetVelocity() const override;

    /// @brief 死亡しているか
    /// @return　死亡しているならtrue
    bool IsDead() const { return isDead_; };

    /// @brief キックされているか取得
    /// @return キックされているならtrue
    bool IsKicked() const override { return isKicked_; };

    /// @brief Particleのポインタをセットする
    /// @param ptr 
    void SetCrashParticlePtr(AppleCrashParticle* ptr) { crashParticle_ = ptr; }
    /// @brief Particleのポインタをセットする
/// @param ptr 
    void SetFlashParticlePtr(FlashParticle* ptr) { flashParticle_ = ptr; }

#ifdef _DEBUG
    /// @brief 編集
    /// @param label ラベル
    void Edit(const std::string& label);
#endif // _DEBUG

private:
    // ワールドトランスフォーム
    WorldTransform worldTransform_;
    // モデル
    std::unique_ptr<Model> model_ = nullptr;
    // カメラ
    Camera* camera_ = nullptr;
    //歩行の速さ
    static inline const float kWalkSpeed = 0.01f;
    //最初の角度
    static inline const float kWalkMotionStart = 0.0f;
    //最後の角度
    static inline const float kWalkMotionAngleEnd = 60.0f;
    //アニメーションの周期となる時間
    static inline const float kWalkMotionTime = 1.0f;
    //経過時間
    float walkTimer_ = 0.0f;
    // キャラクターの当たり判定サイズ
    static inline const float kWidth = 2.0f;
    static inline const float kHeight = 2.0f;
    static inline const float kRadius = 2.0f;
    Vector4 color_;
    // 剛体
    std::unique_ptr<RigidBody> rigidBody_ = nullptr;
    // AABBのデバッグ描画
    std::unique_ptr<AABBRenderer> aabbRenderer_ = nullptr;
    // AABBのデバッグ描画の切り替えフラグ
    bool isExistAABB_ = false;
    // 球のデバッグ描画
    std::unique_ptr<SphereRenderer> sphereRenderer_ = nullptr;
    // 球のデバッグ描画の切り替えフラグ
    bool isExistSphere_ = false;
    // 死亡フラグ
    bool isDead_ = false;
    // キックされたかどうかのフラグ
    bool isKicked_ = false;
    //キック継続時間
    uint32_t kickDurationTimer_ = 0;
    //パーティクルのポインタを取得する
     AppleCrashParticle* crashParticle_ = nullptr;
     FlashParticle* flashParticle_ = nullptr;

};
