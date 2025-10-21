#pragma once
#include "Vector3.h"
#include "Matrix3x3.h"
#include <string>

/// @brief 剛体
class RigidBody {
public:
	static inline const float miu = 0.0f; // 動摩擦係数

	/// @brief コンストラクタ
	RigidBody() = default;

	/// @brief デストラクタ
	~RigidBody() = default;

	/// @brief 初期化
	/// @param mass 質量
	/// @param radius 半径
	void Initialize(float mass, float radius);

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

	/// @brief 重心から力の作用点までのベクトルを設定
	/// @param leverArm 重心から力の作用点までのベクトル
	void SetLeverArm(const Vector3 &leverArm) { leverArm_ = leverArm; }

	/// @brief 速度を取得
	/// @return 速度
	Vector3 GetVelocity() const { return velocity_; }

	/// @brief 角度を取得
	/// @return 角度
	Vector3 GetAngle() const { return angle_; }

	/// @brief 質量を取得
	/// @return 質量
	float GetMass() const { return mass_; }

#ifdef _DEBUG
	/// @brief 編集
	/// @param label ラベル
	void Edit(const std::string &label);
#endif // _DEBUG

private:
	Vector3 velocity_ = {};						// 速度
	Vector3 force_ = {};						// 力
	Vector3 gravity_ = { 0.0f, -9.81f, 0.0f };	// 重力
	Vector3 angle_ = {};						// 角度
	Vector3 angularMomentum_{};					// 角運動量
	Vector3 angularVelocity_ = {};				// 角速度
	Vector3 leverArm_ = {};						// 重心から力の作用点までのベクトル
	Matrix3x3 inertiaTensor_;					// 慣性テンソル
	float mass_ = 1.0f;                         // 質量
	float radius_ = 1.0f;                       // 半径

	/// @brief 球の慣性テンソルを作成する
	/// @param mass 質量
	/// @param radius 半径
	/// @return 慣性テンソル行列
	Matrix3x3 MakeSphereInertiaTensor(float mass, float radius);
};