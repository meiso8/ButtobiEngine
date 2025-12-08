#include "FloorGameFloor.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include"CubeMesh.h"
#include"CollisionConfig.h"
#include "Input.h"
#include "MatsumotoObj/GameSceneObj/Data/MapData.h"
#include "MatsumotoObj/MY_Utility.h"

#include "MatsumotoObj/GameSceneObj/AttackAreaEmitter.h"
#include"VibrateManager.h"
FloorGameFloor::FloorGameFloor() {
	body_.Create();

	models_ =
	{
	  { FloorType::Normal, ModelManager::GetModel(ModelManager::FLOOR)},
		{ FloorType::Sticky,ModelManager::GetModel(ModelManager::FLOOR_FRAME)},
	  { FloorType::Strong, ModelManager::GetModel(ModelManager::HARD_FLOOR) },
	  { FloorType::Bomb, ModelManager::GetModel(ModelManager::EXPLOTION_FLOOR) }
	};

	body_.SetMesh(models_[FloorType::Normal]);

	SetCollisionAttribute(kCollisionFloor);
	SetCollisionMask(kCollisionEnemyBomb|kCollisionEnemy);
	SetRadius(0.5f);

	isExploded_ = false;
	downColor_ = 0.5f;
}

FloorGameFloor::~FloorGameFloor() {
}

void FloorGameFloor::Initialize() {
	body_.Initialize();
	autoSwapTimer_ = 0.0f;
	floorType_ = FloorType::Normal;
	prevFloorType_ = FloorType::Normal;
	autoSwapDuration_ = 6.0f;
	nextFloorType_ = FloorType::Sticky;
	floorTypeUpdate_ = {
		{FloorType::Normal, std::bind(&FloorGameFloor::NormalFloorUpdate, this)},
		{FloorType::Sticky, std::bind(&FloorGameFloor::StickyFloorUpdate, this)},
		{FloorType::Strong, std::bind(&FloorGameFloor::StrongFloorUpdate, this)},
		{FloorType::Bomb, std::bind(&FloorGameFloor::BombFloorUpdate, this)}
	};


	body_.worldTransform_.scale_ = { kHalfFloorSize * 2.0f,kHalfFloorSize * 2.0f,kHalfFloorSize * 2.0f };
	body_.worldTransform_.translate_.y = -0.5f;
	isPopuping_ = false;
	isExploded_ = false;
	isToStrong_ = false;
	//body_.InitWaveData();
	attackAreaEffectID_ = UINT32_MAX;
}

void FloorGameFloor::Update() {

	//ヨシダ　強く復活下かどうかのフラグを戻す　追加しました。
	isToStrong_ = false;

	if (isPopuping_) {
		body_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.translate_.y, -0.5f + 0.1f,0.1f);
		body_.worldTransform_.rotate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.rotate_.y, 31.4f * 0.5f, 0.1f);
		body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(body_.worldTransform_.scale_, 
			{ (kHalfFloorSize * 2.0f) * 1.2f,(kHalfFloorSize * 2.0f) * 1.2f,(kHalfFloorSize * 2.0f) * 1.2f }, 0.1f);
		downColor_ = 1.0f;
	} else {
		body_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.translate_.y, -0.5f, 0.1f);
		body_.worldTransform_.rotate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.rotate_.y, 0.0f, 0.1f);
		body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(body_.worldTransform_.scale_, { kHalfFloorSize * 2.0f,kHalfFloorSize * 2.0f,kHalfFloorSize * 2.0f }, 0.1f);
		downColor_ = 0.83f;
	}

	if (floorType_ != FloorType::Bomb) {
		body_.worldTransform_.rotate_.x = 0.0f;
		body_.worldTransform_.rotate_.z = 0.0f;
	}

	body_.Update();
	//メッシュをセットする
	body_.SetMesh(models_[floorType_]);
	floorTypeUpdate_[floorType_]();

	ColliderUpdate();
}

void FloorGameFloor::Draw(Camera& camera) {
	body_.Draw(camera);
}

void FloorGameFloor::SwapFloorType(FloorType type) {
	if (floorType_ == FloorType::Bomb || type != FloorType::Bomb) {
		AttackAreaEmitter::GetInstance().DeleteEffect(attackAreaEffectID_);
		attackAreaEffectID_ = UINT32_MAX;
	}

	prevFloorType_ = floorType_;
	floorType_ = type;
	
	switch (floorType_)
	{
	case FloorType::Normal:
		nextFloorType_ = FloorType::Sticky;
		break;
	case FloorType::Sticky:
		autoSwapTimer_ = autoSwapDuration_;
		nextFloorType_ = FloorType::Strong;

		break;
	case FloorType::Strong:

		//ヨシダ　強く復活下かどうかのフラグ　追加しました。
		isToStrong_ = true;
		nextFloorType_ = FloorType::Normal;
		break;
	case FloorType::Bomb:
		autoSwapTimer_ = autoSwapDuration_;
		nextFloorType_ = prevFloorType_;
		break;
	default:
		break;
	}
}

void FloorGameFloor::SwapNextFloorType() {
	SwapFloorType(nextFloorType_);
}

void FloorGameFloor::popupFloor() {
	isPopuping_ = true;
}

void FloorGameFloor::downFloor() {
	isPopuping_ = false;
}

void FloorGameFloor::OnCollision(Collider* collider) {
	if (floorType_ == FloorType::Bomb || floorType_ == FloorType::Strong) {
		return;
	}
	if (collider->GetCollisionAttribute() == kCollisionEnemyBomb) {
		SwapFloorType(FloorType::Bomb);
		attackAreaEffectID_ = AttackAreaEmitter::GetInstance().EmitCircle(body_.worldTransform_.GetWorldPosition(), 3.0f, autoSwapDuration_);
	}
}

void FloorGameFloor::NormalFloorUpdate() {
	//body_.InitWaveData();
	body_.SetColor({ downColor_,downColor_,1.0f,1.0f});
}

void FloorGameFloor::StickyFloorUpdate() {

	float t =1.0f- autoSwapTimer_ / autoSwapDuration_;
	t = std::clamp(t, 0.0f, 1.0f);
	body_.SetColor({1.0f ,t, t, 1.0f });

	if (autoSwapTimer_ > 0.0f) {
		autoSwapTimer_ -= 0.016f;
	} else {
		body_.worldTransform_.translate_.y = -1.0f;
		SwapFloorType(nextFloorType_);
	}
}

void FloorGameFloor::StrongFloorUpdate() {
	//body_.InitWaveData();
	body_.SetColor({ 1.0f * downColor_,1.0f * downColor_,1.0f,1.0f });
}

void FloorGameFloor::BombFloorUpdate() {
	//body_.InitWaveData();
	float color = autoSwapTimer_ / autoSwapDuration_;
	color = std::clamp(color, 0.0f, 1.0f);
	float elapsed = autoSwapDuration_ - autoSwapTimer_;
	

	// colorが0に近づくほど点滅が速くなる
	float blinkSpeed = 5.0f + (1.0f - color) * 20.0f;
	float blink = (sinf(elapsed * blinkSpeed) * 0.5f + 0.5f);

	// 点滅効果を掛け合わせる
	body_.SetColor({ 1.0f, color * downColor_ * blink, color * blink, 1.0f });
	


	if (autoSwapTimer_ > 0.0f) {
		autoSwapTimer_ -= 0.016f;
		//振動させる　ヨシダ
		WORD vibrate = 1000+(WORD)color * 1000;
		VibrateManager::SetTime(0.016f, vibrate, vibrate);
	} else {
		SwapFloorType(nextFloorType_);
		Sound::PlaySE(Sound::kExplosion);
		VibrateManager::SetTime(0.5f, 3000, 3000);
		isExploded_ = true;

	}

	float rotateSpeed = 1.0f + (1.0f - color) * 5.0f;
	body_.worldTransform_.rotate_.x = sinf(elapsed * 5.0f * rotateSpeed) * ((1.0f - color) * 0.5f);
	body_.worldTransform_.rotate_.z = cosf(elapsed * 5.0f * rotateSpeed) * ((1.0f - color) * 0.5f);
}
