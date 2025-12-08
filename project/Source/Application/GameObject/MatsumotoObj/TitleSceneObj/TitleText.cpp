#include "TitleText.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include"Collision.h"
#include "Input.h"
#include"CollisionConfig.h"
#include "Cube/CubeMesh.h"
#include "MatsumotoObj/MY_Utility.h"
#include<algorithm>
#include"VibrateManager.h"
TitleText::TitleText() {
	body_.Create();
	body_.Initialize();
	//cubeMesh_ = std::make_unique<CubeMesh>();
	//cubeMesh_->Create();
	model_ = ModelManager::GetModel(ModelManager::ENEMY_SLEEP);
	
	body_.SetMesh(model_);

	body_.worldTransform_.translate_.y = 2.0f;
	body_.worldTransform_.translate_.z = 12.0f;

	SetRadius(5.0f);
	SetCollisionAttribute(kCollisionEnemy);

	hitCount_ = 0;
}

TitleText::~TitleText() {
}

void TitleText::OnCollision(Collider* collider) {
	if (collider->GetCollisionAttribute() == kCollisionPlayerBullet) {
		hitCount_++;
		if (force_.isGravity == true) { return; }
		force_.velocity.y += 0.3f;
		rotatePower_ += 0.5f;
		force_.isGravity = true;
		damageTimer_ = 0.4f;

		VibrateManager::SetTime(0.2f, 3000, 3000);
	}
}

void TitleText::Initialize() {
	damageTimer_ = 0.0f;
	rotatePower_ = 0.0f;
	force_.velocity = { 0.0f,0.0f,0.0f };
	force_.isGravity = false;
	hitCount_ = 0;

	body_.worldTransform_.translate_ = { 0.0f,2.0f,12.0f };
	body_.worldTransform_.scale_ = { 5.0f,5.0f,5.0f };
	body_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
	timer_ = 0.0f;
	isBreak_ = false;
}

void TitleText::Update() {
	if (isBreak_) { return; }
	if (hitCount_ >= maxHitCount_) {
		isBreak_ = true;
		return;
	}

	float hitScale = 5.0f + static_cast<float>(hitCount_) * 0.5f;
	timer_ += 0.016f;
	if (damageTimer_ > 0.0f) {
		damageTimer_ -= 0.016f;
		//body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(body_.worldTransform_.scale_, { hitScale * 0.5f,hitScale * 2.0f,hitScale * 0.5f }, 0.5f);

		if (MY_Utility::IsActiveForTimerSwitch(damageTimer_, 0.1f)) {
			body_.SetColor({1.0f,0.0f,0.0f,1.0f});
		} else {
			body_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}

	} else {
		body_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
		body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(body_.worldTransform_.scale_,{ hitScale,hitScale,hitScale }, 0.3f);
	}

	body_.worldTransform_.translate_.x = sinf(timer_ * (hitScale - 1.0f) * 10.0f) * 0.1f;

	force_.Update();
	//body_.worldTransform_.rotate_.y += rotatePower_;
	rotatePower_ *= 0.98f;
	body_.worldTransform_.translate_ += force_.velocity;
	if (body_.worldTransform_.translate_.y < 0.0f) {
		force_.isGravity = false;
		body_.worldTransform_.translate_.y = 0.0f;
	}
	body_.Update();
	
	ColliderUpdate();
}

void TitleText::Draw(Camera& camera, const LightMode& lightType) {
	body_.SetLightMode(lightType);
	body_.Draw(camera, kBlendModeNormal);
}
