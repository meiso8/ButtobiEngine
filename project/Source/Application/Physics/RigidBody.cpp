#include "RigidBody.h"
#include "MakeMatrix.h"
#include "CoordinateTransform.h"
#include <cmath>
#include <numbers>
#include <algorithm>

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG

void RigidBody::Initialize(float mass, float radius) {
	mass_ = mass;		// 質量を設定
	radius_ = radius;	// 半径を設定
	inertiaTensor_ = MakeSphereInertiaTensor(mass_, radius_);	// 慣性テンソルを計算
}

void RigidBody::Update(float deltaTime) {
	// 摩擦力の計算
	float magnitude = miu * Length(-mass_ * gravity_);
	Vector3 direction = Normalize(velocity_);
	Vector3 frictionalForce = -magnitude * direction;
	force_ += frictionalForce;

	// 重力を力に加える
	force_ += gravity_ * mass_;

	// 加速度 = 力 / 質量
	Vector3 acceleration = force_ / mass_;

	// 速度に加速度を加える
	velocity_ += acceleration * deltaTime;
	
	// トルクを計算
	Vector3 torque = Cross(leverArm_, force_);
	
	// 慣性テンソルの逆行列を計算
	Matrix3x3 inertiaTensorInverse = Inverse(inertiaTensor_);

	// 角運動量を更新
	angularMomentum_ += torque * deltaTime;

	// 角速度を更新
	angularVelocity_ = CoordinateTransform(angularMomentum_, inertiaTensorInverse);
	angularVelocity_.x = std::clamp(angularVelocity_.x, -2.0f * std::numbers::pi_v<float>, 2.0f * std::numbers::pi_v<float>);
	angularVelocity_.y = std::clamp(angularVelocity_.y, -2.0f * std::numbers::pi_v<float>, 2.0f * std::numbers::pi_v<float>);
	angularVelocity_.z = std::clamp(angularVelocity_.z, -2.0f * std::numbers::pi_v<float>, 2.0f * std::numbers::pi_v<float>);

	// 角度を更新
	angle_ += angularVelocity_ * deltaTime;
	angle_.x = std::fmod(angle_.x, 2.0f * std::numbers::pi_v<float>);
	angle_.y = std::fmod(angle_.y, 2.0f * std::numbers::pi_v<float>);
	angle_.z = std::fmod(angle_.z, 2.0f * std::numbers::pi_v<float>);

	// 力をリセット
	force_ = {};
}

#ifdef _DEBUG
void RigidBody::Edit(const std::string &label) {
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::DragFloat3("force", &force_.x, 0.01f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		ImGui::TreePop();
	}
}
#endif // _DEBUG

Matrix3x3 RigidBody::MakeSphereInertiaTensor(float mass, float radius) {
	Matrix3x3 inertiaTensor = MakeIdentity3x3();			// 単位行列で初期化
	float inertia = (2.0f / 5.0f) * mass * radius * radius;	// 球の慣性テンソル
	inertiaTensor = inertiaTensor * inertia;				// 対角成分に慣性テンソルを設定
	return inertiaTensor;
}