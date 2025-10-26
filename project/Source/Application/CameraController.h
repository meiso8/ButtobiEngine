#pragma once
#include"WorldTransform.h"
#include<cstdint>
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
	void ZoomIn();

	void Reset();

	/// @brief 描画はデバック用にあると便利

	/// @brief 	対処の設定
	/// @param target 対象
	void SetTarget(Player* target) { target_ = target; };

	bool zoomEnd_ = false;
private:
	// カメラ
	Camera* camera_ = nullptr;
	Player* target_ = nullptr;
	// 追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0.0f, 12.5f, 10.0f};
	// カメラの目標座標
	Vector3 targetPos_;
	uint32_t zoomInTimer_ = 0;

};