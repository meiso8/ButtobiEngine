#include "RigidBody.h"
#include "Length.h"
#include "Normalize.h"

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG


void RigidBody::Update(float deltaTime) {
	// 摩擦力の計算
	float magnitude = miu * Length(-mass_ * gravity_);
	Vector3 direction = Normalize(-velocity_);
	Vector3 frictionalForce = magnitude * direction;
	force_ += frictionalForce;

	// 重力を力に加える
	force_ += gravity_ * mass_;

	// 加速度 = 力 / 質量
	Vector3 acceleration = force_ / mass_;

	// 速度に加速度を加える
	velocity_ += acceleration * deltaTime;

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
