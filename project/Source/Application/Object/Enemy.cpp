#define NOMINMAX
#include "Enemy.h"
#include "AABB.h"
#include "Sphere.h"
#include "Player.h" // 追加
#include "Model.h"
#include "RigidBody.h"
#include "Normalize.h"
#include <cmath>
#include <numbers>
#include <cassert>

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG


Enemy::Enemy() = default;

Enemy::~Enemy() {
	delete model_;
}

void Enemy::Initialize(Vector3 &position) {

	model_ = new Model();
	model_->Create(ModelManager::ENEMY);
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	worldTransform_.Initialize();
	worldTransform_.translate_ = position; // 初期位置をオリジンにしておく
	worldTransform_.rotate_.y = std::numbers::pi_v<float> *3.0f / 2.0f;

	// 速度を設定する
	velocity_ = { -kWalkSpeed, 0, 0 };
	walkTimer_ = 0.0f;

	// 剛体の生成
	rigidBody_ = std::make_unique<RigidBody>();

	// AABB描画の生成
	aabbRenderer_ = std::make_unique<AABBRenderer>();
	aabbRenderer_->Initialize();

	// 球描画の生成
	sphereRenderer_ = std::make_unique<SphereRenderer>();
	sphereRenderer_->Initialize();
}

void Enemy::Update() {

	// 移動
	/*worldTransform_.translate_ += velocity_;*/

	walkTimer_ += 1.0f / 60.0f;
	// 回転アニメーション
	float param = std::sin(std::numbers::pi_v<float> *walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotate_.x = radian * std::numbers::pi_v<float> / 180.0f;
	rigidBody_->Update(1.0f / 60.0f);
	worldTransform_.translate_ += rigidBody_->GetVelocity() / 60.0f;

	// ==============================
	// 行列を定数バッファに転送
	// ==============================

	WorldTransformUpdate(worldTransform_);

	aabbRenderer_->SetAABB(GetAABB());
	aabbRenderer_->Update();

	sphereRenderer_->SetSphere(GetSphere());
	sphereRenderer_->Update();
}

void Enemy::Draw(Camera &camera) {
	// 球の描画
	sphereRenderer_->Draw(camera);

	// AABBの描画
	aabbRenderer_->Draw(camera);

	// 3Dモデル描画前処理
	model_->PreDraw(BlendMode::kBlendModeNormal);
	model_->SetColor(color_);
	// 3Dモデルを描画
	model_->Draw(camera, worldTransform_.matWorld_, MaterialResource::LIGHTTYPE::HALF_L);

}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = { worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f };
	aabb.max = { worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f };

	return aabb;
}

Sphere Enemy::GetSphere() {
	Sphere sphere;
	sphere.center = GetWorldPosition();
	sphere.radius = kWidth / 2.0f;
	return sphere;
}

Vector3 Enemy::GetWorldPosition() { return { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] }; }

void Enemy::OnCollision(Player *player) {

	if (player->IsAttack()) {
		color_ = { 0.0f,0.0f,1.0f,1.0f };
	}

	Vector3 normal = Normalize(GetWorldPosition() - player->GetWorldPosition());
	rigidBody_->ApplyForce(normal * 1000.0f);
	// 無意味な処理を入れることで警告を抑制できる

}

#ifdef _DEBUG
void Enemy::Edit(const std::string &label) {
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::DragFloat3("translate", &worldTransform_.translate_.x, 0.01f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		rigidBody_->Edit("RigidBody");
		aabbRenderer_->Edit("AABB");
		ImGui::TreePop();
	}
}
#endif // _DEBUG