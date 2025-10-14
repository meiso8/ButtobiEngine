#pragma once
#include "Vector3.h"
#include <string>

/// @brief 剛体
class RigidBody {
public:
	static inline const float miu = 0.2f; // 動摩擦係数

	/// @brief コンストラクタ
	RigidBody() = default;

	/// @brief デストラクタ
	~RigidBody() = default;

	/// @brief 更新
	/// @param deltaTime デルタタイム(秒)
	void Update(float deltaTime);

	/// @brief 力を加える
	/// @param force 加える力
	void ApplyForce(const Vector3 &force) { force_ += force; }

	/// @brief 重力を設定
	/// @param gravity 重力
	void SetGravity(const Vector3 &gravity) { gravity_ = gravity; }

	/// @brief 速度を設定
	/// @param velocity 速度
	void SetVelocity(const Vector3 &velocity) { velocity_ = velocity; }

	/// @brief 速度を取得
	/// @return 速度
	Vector3 GetVelocity() const { return velocity_; }

#ifdef _DEBUG
	/// @brief 編集
	/// @param label ラベル
	void Edit(const std::string &label);
#endif // _DEBUG


private:
	Vector3 velocity_ = {};						// 速度
	Vector3 force_ = {};						// 力
	Vector3 gravity_ = { 0.0f, -9.81f, 0.0f };	// 重力
	float mass_ = 1.0f;                         // 質量
};