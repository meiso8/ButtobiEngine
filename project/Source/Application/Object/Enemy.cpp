#define NOMINMAX
#include "Enemy.h"
#include "AABB.h"
#include "Sphere.h"
#include "Player.h" // 追加
#include "Model.h"
#include "RigidBody.h"
#include "CollisionConfig.h"
#include "Collision.h"
#include <cmath>
#include <numbers>
#include <cassert>

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#endif // _DEBUG


Enemy::Enemy() {
	model_ = new Model();
	model_->Create(ModelManager::FRUIT_APPLE);
};

Enemy::~Enemy() {
	delete model_;
}

void Enemy::Initialize(Vector3 &position) {

	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	worldTransform_.Initialize();
	worldTransform_.translate_ = position; // 初期位置をオリジンにしておく
	worldTransform_.rotate_.y = std::numbers::pi_v<float> *3.0f / 2.0f;

	// 速度を設定する
	velocity_ = { -kWalkSpeed, 0, 0 };
	walkTimer_ = 0.0f;

	// 剛体の生成
	rigidBody_ = std::make_unique<RigidBody>();

#ifdef _DEBUG
	// AABBのデバッグ描画の生成と初期化
	isExistAABB_ = false;
	aabbRenderer_ = std::make_unique<AABBRenderer>();
	aabbRenderer_->Initialize();

	// 球のデバッグ描画の生成と初期化
	isExistSphere_ = false;
	sphereRenderer_ = std::make_unique<SphereRenderer>();
	sphereRenderer_->Initialize();
#endif // _DEBUG
}

void Enemy::Update() {
	walkTimer_ += 1.0f / 60.0f;
	// 回転アニメーション
	float param = std::sin(std::numbers::pi_v<float> *walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotate_.x = radian * std::numbers::pi_v<float> / 180.0f;

	// 移動
	rigidBody_->Update(1.0f / 60.0f);
	worldTransform_.translate_ += rigidBody_->GetVelocity() / 60.0f;

	// ==============================
	// 行列を定数バッファに転送
	// ==============================

	WorldTransformUpdate(worldTransform_);

#ifdef _DEBUG
	// AABBのデバッグ描画の更新
	if (isExistAABB_) {
		aabbRenderer_->Update(GetAABB());
	}

	// 球のデバッグ描画の更新
	if (isExistSphere_) {
		sphereRenderer_->Update(GetSphere());
	}
#endif // _DEBUG
}

void Enemy::Draw(Camera &camera) {
#ifdef _DEBUG
	// 球のデバッグ描画
	if (isExistSphere_) {
		sphereRenderer_->Draw(camera);
	}

	// AABBのデバッグ描画
	if (isExistAABB_) {
		aabbRenderer_->Draw(camera);
	}
#endif // _DEBUG

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

Vector3 Enemy::GetWorldPosition() const { return { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] }; }

void Enemy::OnCollision(Player *player) {
	if (player->IsAttack()) {
		rigidBody_->ApplyForce(player->GetKickForce());
	}
}

void Enemy::OnCollision(const Plane &plane) {
	constexpr float exz = 1.0f;												// XZ軸の反発係数
	constexpr float ey = 0.8f;												// Y軸の反発係数
	float distance = Distance(GetSphere(), plane);							// 球の中心から平面までの距離
	float penetration = GetSphere().radius - distance;						// 貫入量を球の半径に設定
	Vector3 reflected = Reflect(rigidBody_->GetVelocity(), plane.normal);	// 反射ベクトルの計算
	Vector3 projectNormal = Project(reflected, plane.normal);				// 法線方向の投影
	Vector3 movingDirection = reflected - projectNormal;					// 移動方向の計算
	Vector3 velocity;														// フルーツの速度
	velocity.x = projectNormal.x * exz + movingDirection.x;
	velocity.y = projectNormal.y * ey + movingDirection.y;
	velocity.z = projectNormal.z * exz + movingDirection.z;
	rigidBody_->SetVelocity(velocity);							// 衝突後のフルーツの速度を更新
	worldTransform_.translate_ += projectNormal * penetration;	// 貫入量分フルーツの位置を修正
}

void Enemy::OnCollision(const OBB &obb) {
	constexpr float exz = 1.0f;										// XZ軸の反発係数
	constexpr float ey = 0.8f;										// Y軸の反発係数
	float distance = Distance(GetSphere(), obb);					// 球の中心から平面までの距離
	float penetration = GetSphere().radius - distance;              // 球の半径と距離の差分を貫入量に設定
	Vector3 closestPoint = ClosestPoint(GetSphere().center, obb);	// 球の中心から最も近い点を取得
	Vector3 normal = Normalize(GetSphere().center - closestPoint);	// 法線ベクトルの計算
	Vector3 reflected = Reflect(rigidBody_->GetVelocity(), normal);	// 反射ベクトルの計算
	Vector3 projectNormal = Project(reflected, normal);				// 法線方向の投影
	Vector3 movingDirection = reflected - projectNormal;            // 移動方向の計算
	Vector3 velocity;												// フルーツの速度
	velocity.x = projectNormal.x * exz + movingDirection.x;
	velocity.y = projectNormal.y * ey + movingDirection.y;
	velocity.z = projectNormal.z * exz + movingDirection.z;
	rigidBody_->SetVelocity(velocity);							// 衝突後のフルーツの速度を更新
	worldTransform_.translate_ += projectNormal * penetration;	// 貫入量分フルーツの位置を修正
}

void Enemy::OnCollision() {
	isDead_ = true;
}

#ifdef _DEBUG
void Enemy::Edit(const std::string &label) {
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::DragFloat3("translate", &worldTransform_.translate_.x, 0.01f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		rigidBody_->Edit("RigidBody");
		ImGui::Checkbox("DrawAABB", &isExistAABB_);
		ImGui::Checkbox("DrawSphere", &isExistSphere_);
		ImGui::TreePop();
	}
}
#endif // _DEBUG