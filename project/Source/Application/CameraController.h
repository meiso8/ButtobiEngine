#pragma once
#include"WorldTransform.h"

class Player;
class Camera;

/// @brief カメラ操作
class CameraController {

	// 矩形
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

public:
	/// @brief 初期化
	void Initialize(Camera* camera);

	/// @brief 更新
	void Update();

	void Reset();

	/// @brief 描画はデバック用にあると便利

	/// @brief 	対処の設定
	/// @param target 対象
	void SetTarget(Player* target) { target_ = target; };
	/// @brief 移動範囲のセッター
	/// @param area 範囲
	void SetMovableArea(Rect area) { movableArea_ = area; };

private:
	// 座標補間割合
	static inline const float kInterpolationRate = 0.5f;
	// 速度掛け率
	static inline const float kVelocityBias = 2.0f;
	//追従対象の各方向へのカメラ移動範囲
	static inline const Rect margin = {-10.0f, 10.0f, -5.0f, 5.0f};
	// カメラ
	Camera* camera_ = nullptr;
	Player* target_ = nullptr;
	// 追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0.0f, 10.0f, 15.0f};
	// カメラの移動範囲
	Rect movableArea_ = {0.0f, 100.0f, 0.0f, 100.0f};
	// カメラの目標座標
	Vector3 targetPos_;
};