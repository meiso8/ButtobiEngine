#include "HealItem.h"
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

HealItem::HealItem() {
	model_ = ModelManager::GetModel(ModelManager::HEAL_ITEM);
	body_.Create();
	body_.Initialize();
	body_.SetMesh(model_);
	body_.worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
	//コライダーの設定
	SetRadius(0.7f);
	SetCollisionAttribute(kCollisionPlayerHealItem);
	SetCollisionMask(kCollisionPlayer);
	isActive_ = false;
}

HealItem::~HealItem() {
}

void HealItem::OnCollision(Collider* collider) {
	if (!isActive_) {
		return;
	}
	SetCollisionAttribute(kCollisionNone);
	isActive_ = false;
}

void HealItem::Initialize() {
	isActive_ = false;
	rotateSpeed_ = 1.0f;
}

void HealItem::Update() {
	if (isActive_) {
		body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(
			body_.worldTransform_.scale_,
			{ 1.0f,1.0f,1.0f },
			0.1f
		);
	} else {
		body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(
			body_.worldTransform_.scale_,
			{ 0.0f,0.0f,0.0f },
			0.1f
		);
	}
	
	body_.worldTransform_.rotate_.y += 0.016f * rotateSpeed_;
	body_.Update();
	ColliderUpdate();
}

void HealItem::Draw(Camera& camera) {
	if ( Length(body_.worldTransform_.scale_) <= 0.1f ) {
		return;
	}
	body_.Draw(camera, kBlendModeNormal);
}

void HealItem::Spawn(const Vector3& position) {
	isActive_ = true;
	body_.worldTransform_.translate_ = position;
	body_.worldTransform_.scale_ = { 0.0f,0.0f,0.0f };
	body_.SetColor({ 0.0f,1.0f,0.0f,1.0f });
	SetCollisionAttribute(kCollisionPlayerHealItem);
}
