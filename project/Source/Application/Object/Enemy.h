#pragma once
#include"WorldTransform.h"
#include"Vector4.h"

struct AABB;

class Player;
class Model;
class Camera;

/// @brief 敵
class Enemy {

public:
	void Initialize(Model* model, Camera* camera, Vector3& position);
	void Update();
	void Draw();
	AABB GetAABB();
	// ワールド座標を取得
	Vector3 GetWorldPosition();
	void OnCollision(const Player* player);

private:
	// ワールドトランスフォーム
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;
	//歩行の速さ
	static inline const float kWalkSpeed = 0.01f;
	//速度
	Vector3 velocity_ = {};
	//最初の角度
	static inline const float kWalkMotionStart = 0.0f;
	//最後の角度
	static inline const float kWalkMotionAngleEnd = 60.0f;
	//アニメーションの周期となる時間
	static inline const float kWalkMotionTime = 1.0f;
	//経過時間
	float walkTimer_ = 0.0f;
	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;
	Vector4 color_;
};
