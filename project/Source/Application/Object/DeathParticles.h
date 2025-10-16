#pragma once
#include"WorldTransform.h"
#include"Vector4.h"

#include <cmath>
#include <numbers>
#include<array>

class Camera;
class Model;

/// @brief  デス演出用パーティクル
class DeathParticles {
public:
	void Initialize(const Vector3& position);
	void Update();
	void Draw(Camera& camera);
	~DeathParticles();
private:
	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;
	// パーティクルの個数
	static inline const uint32_t kNumParticles = 8;
	// ワールド変換データ パーティクルの数h固定のため固定長配列で持つ
	std::array<WorldTransform, kNumParticles> worldTransforms_;
	// 存続時間（消滅までの時間）<秒>
	static inline const float kDuration = 0.5f;
	// 移動の速さ
	static inline const float kSpped = 0.125f;
	// 分割した一1個分の角度
	static inline const float kAngleUnit = 2.0f * std ::numbers::pi_v<float> / kNumParticles;
	// 終了フラグ
	bool isFinished_ = false;
	// 経過時間カウント
	float counter_ = 0.0f;
	// 色の取得
	Vector4 color_;
};