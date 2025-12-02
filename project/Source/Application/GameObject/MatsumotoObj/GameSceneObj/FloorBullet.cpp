#include "FloorBullet.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"

#include"Data/FloorData.h"

FloorBullet::FloorBullet() {
	body_.Create();
	body_.SetColor({ 1.0f,0.0f,0.0f,1.0f });
	body_.SetMesh(cubeMesh_.get());

	models_ =
	{
	  { FloorType::Normal, ModelManager::GetModel(ModelManager::FLOOR)},
	  { FloorType::Sticky, ModelManager::GetModel(ModelManager::MELT_FLOOR) },
	  { FloorType::Strong, ModelManager::GetModel(ModelManager::FLOOR) },
	  { FloorType::Bomb, ModelManager::GetModel(ModelManager::FLOOR) }
	};

	body_.SetMesh(models_[FloorType::Normal]);


	SetRadius(0.5f);
	SetCollisionAttribute(kCollisionPlayerBullet);
	// 弾は「敵と敵の弾」とだけ衝突したい
	SetCollisionMask(kCollisionEnemy | kCollisionEnemyBullet);
}

FloorBullet::~FloorBullet() {
}

void FloorBullet::Initialize() {
	body_.Initialize();
	moveDir_ = { 0.0f,0.0f,1.0f };
	moveSpeed_ = 0.2f;
	lifeTimer_ = 0.0f;
	lifeDuration_ = 2.0f;
	isActive_ = false;
	isHit_ = false;
	size_ = 1.0f;
	velocity_ = { 0.0f,0.0f,0.0f };

	floorType_ = FloorType::Normal;
	reqestFloorType_ = FloorType::Normal;
}
void FloorBullet::InitFlagAndPosAndTimer() {
	lifeTimer_ = 0.0f;
	body_.worldTransform_.translate_ = { 0.0f,-10.0f,0.0f };
	isActive_ = false;
}
void FloorBullet::OnCollision(Collider* collider) {


	if (!isActive_ || isHit_) {
		return;
	}

	if (collider->GetCollisionAttribute() == kCollisionEnemy || collider->GetCollisionAttribute() == kCollisionEnemyBullet) {
		//デバック用
		OnCollisionCollider();
	}

	if (collider->GetCollisionAttribute() == kCollisionEnemy) {
		//デバック用
		OnCollisionCollider();

		if (floorType_ == FloorType::Strong) {
			if (!isHoming_) {
				FloorFlag::isHitMultipleFloor = true;
			}
		}
	}
	isHit_ = true;

}
Vector3 FloorBullet::GetWorldPosition() const {
	return body_.worldTransform_.GetWorldPosition();
}
void FloorBullet::Update() {

#ifdef USE_IMGUI

	ImGui::Begin("bullet");
	if (ImGui::TreeNode("Floorbullet")) {
		DebugUI::CheckFlag(isActive_, "isActive");
		DebugUI::CheckFlag(isHit_, "isHit_");
		ImGui::Text(" lifeTimer_ %f", lifeTimer_);
		DebugUI::CheckWorldTransform(body_.worldTransform_, "BulletTransform");
		ImGui::TreePop();
	}
	ImGui::End();


#endif

	if (!isActive_) {
		return;
	}

	if (isHit_) {
		InitFlagAndPosAndTimer();
		isHit_ = false;
	}

	if (isHoming_) { HomingMove(); }
	Move();
	body_.Update();
	ColliderUpdate();

}

void FloorBullet::Draw(Camera& camera, const LightMode& lightType) {

	if (!isActive_) {
		return;
	}

	if (floorType_ != reqestFloorType_)
	{
		floorType_ = reqestFloorType_;
		body_.SetMesh(models_[floorType_]);
	}

	body_.SetLightMode(lightType);
	body_.Draw(camera, kBlendModeNormal);

}

void FloorBullet::Shot(const Vector3& position, const Vector3& direction, const float& speed, const float& size, FloorType type) {
	body_.worldTransform_.translate_ = position;
	moveDir_ = Normalize(direction);
	moveSpeed_ = speed;
	size_ = size;
	lifeTimer_ = lifeDuration_;
	isActive_ = true;
	isHit_ = false;
	isHoming_ = false;
	reqestFloorType_ = type;


	//一フレーム分遅れているためここで更新しました。　ヨシダ
	body_.Update();
	ColliderUpdate();
}

void FloorBullet::SetHoming(Object3d* target, float stayTime) {
	if (!isHoming_) {
		isHoming_ = true;
		homingTarget_ = target;
		homingStayTime_ = stayTime;

		velocity_ = (body_.worldTransform_.GetWorldPosition() - homingTarget_->worldTransform_.GetWorldPosition());
		float dist = Length(velocity_);
		velocity_.y = 0.0f;
		velocity_ = Normalize(velocity_) * dist * 0.1f;
	}
}


void FloorBullet::Move() {
	if (isHoming_) {
		if (homingStayTime_ > 0.0f) {
			homingStayTime_ -= 0.016f;
			body_.worldTransform_.rotate_.y += 0.5f;
			body_.worldTransform_.translate_ += velocity_;
			velocity_ *= 0.95f;
			return;
		}
	}

	if (lifeTimer_ <= 0.0f) {
		InitFlagAndPosAndTimer();
		return;
	} else {
		lifeTimer_ -= 0.016f;
	}

	
	if (isHoming_) {
		body_.worldTransform_.translate_ += moveDir_ * moveSpeed_ * 3.0f;
	} else {
		body_.worldTransform_.rotate_.y = atan2f(moveDir_.x, moveDir_.z);
		body_.worldTransform_.translate_ += moveDir_ * moveSpeed_;
	}
}

void FloorBullet::HomingMove() {
	if (homingTarget_ == nullptr) { return; }
	moveDir_ = Normalize(homingTarget_->worldTransform_.GetWorldPosition() - body_.worldTransform_.GetWorldPosition());
}
