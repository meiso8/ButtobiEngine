#include "Enemy.h"
#include "AABB.h"
#include "Player.h" // 追加
#include"Model.h"
#include <cmath>
#include <numbers>
#include<cassert>

void Enemy::Initialize(Camera* camera, Vector3& position) {

	model_ = new Model();
	model_->Create(ModelManager::ENEMY);
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translate_ = position; // 初期位置をオリジンにしておく
	worldTransform_.rotate_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;

	// 速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;

}

void Enemy::Update() {

	// 移動
	/*worldTransform_.translate_ += velocity_;*/

	walkTimer_ += 1.0f / 60.0f;
	// 回転アニメーション
	float param = std::sin(std::numbers::pi_v<float> * walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotate_.x = radian * std::numbers::pi_v<float> / 180.0f;

	// ==============================
	// 行列を定数バッファに転送
	// ==============================

	WorldTransformUpdate(worldTransform_);
}

void Enemy::Draw() {

	// 3Dモデル描画前処理
	model_->PreDraw(BlendMode::kBlendModeNormal);
	model_->SetColor(color_);
	// 3Dモデルを描画
	model_->Draw( *camera_, worldTransform_.matWorld_, MaterialResource::LIGHTTYPE::HALF_L);

}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

Vector3 Enemy::GetWorldPosition() { return {worldTransform_.translate_.x, worldTransform_.translate_.y, worldTransform_.translate_.z}; }

void Enemy::OnCollision(const Player* player) {

	if (player->IsAttack()) {
		color_ = { 0.0f,0.0f,1.0f,1.0f };
	}

	// 無意味な処理を入れることで警告を抑制できる
	
}
Enemy::~Enemy()
{
	delete model_;
}
;
