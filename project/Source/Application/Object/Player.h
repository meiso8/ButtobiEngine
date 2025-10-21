#pragma once
#include<array>
#include<memory>

#include"Vector3.h"
#include"Vector4.h"
#include"WorldTransform.h"

struct AABB;

class Enemy;
class Camera;
class Model;
class AABBRenderer;

/// @brief 自キャラ
class Player {

public:
	enum class LRDirection { kRight, kLeft };

	struct CollisionMapInfo {
		bool isCeilingHit = false; // 天井
		bool isLanding = false;    // 着地
		bool isWallHit = false;    // 壁
		Vector3 moveVol;
	};

	// 角
	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,
		kNumCorner // 要素
	};

	enum AttackPhase {
		kNone,
		kCharge,
		kFire,
		kEnd
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

public:
	Player();
	~Player();
	/// @brief 初期化
	/// @param model モデル
	/// @param textureHandle テクスチャハンドル
	void Initialize(Camera &camera, const Vector3 &position);

	/// @brief 更新
	void Update();

	/// @brief 移動入力
	void InputMove();

	/// @brief マップ衝突判定
	/// @param info 衝突情報
	void CheckCollisionMap(CollisionMapInfo &info);

	// @brief 攻撃入力
	void InputAttack();

	// @brief 攻撃アニメーション
	void AttackAnimation();

	void CheckCollisionTop(CollisionMapInfo &info);
	void CheckCollisionBottom(CollisionMapInfo &info);
	void CheckCollisionRight(CollisionMapInfo &info);
	void CheckCollisionLeft(CollisionMapInfo &info);

	Vector3 CornerPosition(const Vector3 &center, Corner corner);

	// 判定結果を反映して移動させる
	void ApplyResultAndMove(const CollisionMapInfo &info);
	/// @brief 天井に接触している場合の処理
	/// @param info
	void CeilingHit(const CollisionMapInfo &info);

	// 設置状態切り替え処理
	void SwitchOnGround(const CollisionMapInfo &info);

	/// @brief 壁に接触している場合の処理
	void WallHit(const CollisionMapInfo &info);

	/// @brief 描画
	void Draw(Camera &camera);

	const WorldTransform &GetWorldTransform() const { return worldTransform_; };
	const Vector3 &GetVelocity() const { return velocity_; };

	/// @brief 前方向を取得
	/// @return 前方向
	Vector3 GetForward() const;

	//ワールド座標を取得
	Vector3 GetWorldPosition() const;

	//AABBを取得する関数
	AABB GetAABB();

	void OnCollision(const Enemy *enemy);

	bool IsAttack()const;
	bool IsCharge()const;

	void InitializeLife(int SetMaxLife) { life_ = SetMaxLife; };

	int GetLife() const { return life_; };

	/// @brief キック力を取得
	/// @return キック力
	Vector3 GetKickForce() const { return kickForce_; };

	//ImGUi用
	void Debug();
private:
	Camera *camera_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;

	WorldTransform PartsWorldTransform_[Parts::kNumParts];
	WorldTransform DrawPartsWorldTransform_[Parts::kNumParts];

	// モデル
	std::array<Model *, Parts::kNumParts> model_ = { nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	// 速度
	Vector3 velocity_ = {};
	// 加速度
	static inline const float kAcceleration = 0.125f;
	// 速度減衰率
	static inline const float kAttenuation = 0.25f;
	// 着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.125f;
	// 壁接触時の速度減衰率
	static inline const float kAttenuationWall = 0.25f;
	// 最大速度制限
	static inline const float kLimitRunSpeed = 1.0f;
	// 方向
	LRDirection lrDirection_ = LRDirection::kRight;
	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー 角度の補間するため
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 接地状態フラグ
	bool onGround_ = true;
	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 9.8f / 120.0f;
	// 最大落下速度
	static inline const float kLimitFallSpeed = 2.0f;
	// ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 1.0f;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;

	static inline const float kBlank = 0.2f;

	// チャージ最大時間
	static inline constexpr float kMaxChargeTime = 10000.0f;

	AttackPhase attackPhase_ = AttackPhase::kNone;

	Vector3 targetPartsScale_[4][Parts::kNumParts];
	Vector3 targetPartsRotate_[4][Parts::kNumParts];
	Vector3 targetPartsTranslate_[4][Parts::kNumParts];

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

	// AABB描画
	std::unique_ptr<AABBRenderer> aabbRenderer_ = nullptr;

	void ResetAttack();
};