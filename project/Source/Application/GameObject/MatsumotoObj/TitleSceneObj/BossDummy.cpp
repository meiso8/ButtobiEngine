#include "BossDummy.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include "Input.h"
#include"CollisionConfig.h"
#include"JsonFile.h"
#include "MatsumotoObj/MY_Utility.h"

BossDummy::BossDummy() {
	body_.Create();
	model_ = ModelManager::GetModel(ModelManager::ENEMY);
	body_.SetMesh(model_);
}

BossDummy::~BossDummy() {
}

void BossDummy::Initialize() {
	body_.worldTransform_.translate_ = { 0.0f,1.5f,8.0f };
	body_.worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
	body_.worldTransform_.scale_ = { 3.0f,3.0f,3.0f };
	body_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
	timer_ = 0.0f;
	isAnimEnd_ = false;
}

void BossDummy::Update() {
	timer_ += 0.016f;
	body_.worldTransform_.rotate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.rotate_.y,3.14f, 0.1f);
	body_.SetColor(MY_Utility::SimpleEaseIn(body_.GetColor(), { 1.0f,0.0f,0.0f,1.0f }, 0.1f));
	body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(body_.worldTransform_.scale_, { 4.0f,4.0f,4.0f }, 0.05f);
	body_.worldTransform_.translate_.x = sinf(timer_*30.0f) * 0.1f;
	body_.Update();

	if (timer_ > 3.0f) {
		isAnimEnd_ = true;
	}
}

void BossDummy::Draw(Camera& camera, const LightMode& lightType) {
	body_.SetLightMode(lightType);
	body_.Draw(camera, kBlendModeNormal);
}
