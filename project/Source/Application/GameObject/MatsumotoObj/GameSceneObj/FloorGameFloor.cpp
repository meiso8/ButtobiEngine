#include "FloorGameFloor.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include"CubeMesh.h"
#include"Collision.h"
#include"CollisionConfig.h"
#include "Input.h"
#include "MatsumotoObj/GameSceneObj/Data/MapData.h"
#include "MatsumotoObj/MY_Utility.h"

FloorGameFloor::FloorGameFloor() {
	body_.Create();

	models_ =
	{
	  { FloorType::Normal, ModelManager::GetModel(ModelManager::FLOOR)},
	  { FloorType::Sticky, ModelManager::GetModel(ModelManager::MELT_FLOOR) },
	  { FloorType::Strong, ModelManager::GetModel(ModelManager::HARD_FLOOR) },
	  { FloorType::Bomb, ModelManager::GetModel(ModelManager::EXPLOTION_FLOOR) }
	};

	body_.SetMesh(models_[FloorType::Normal]);

	SetCollisionAttribute(kCollisionFloor);
	SetCollisionMask(kCollisionEnemyBomb);
	SetRadius(0.5f);

	isExploded_ = false;
	downColor_ = 0.2f;
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

	isExploded_ = false;

	body_.InitWaveData();
}

void FloorGameFloor::Update() {
	if (isPopuping_) {
		body_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.translate_.y, -kHalfFloorSize + 0.1f,0.1f);
		body_.worldTransform_.rotate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.rotate_.y, 31.4f * 0.5f, 0.1f);
		body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(body_.worldTransform_.scale_, { 1.5f,1.5f,1.5f }, 0.1f);
		body_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
		downColor_ = 0.0f;
	} else {
		body_.worldTransform_.translate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.translate_.y, -kHalfFloorSize, 0.1f);
		body_.worldTransform_.rotate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.rotate_.y, 0.0f, 0.1f);
		body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(body_.worldTransform_.scale_, { 1.0f,1.0f,1.0f }, 0.1f);
		body_.SetColor({ 1.0f - downColor_,1.0f - downColor_,1.0f - downColor_,1.0f });
		downColor_ = 0.4f;
	}

	body_.Update();
	//メッシュをセットする
	body_.SetMesh(models_[floorType_]);
	floorTypeUpdate_[floorType_]();

	ColliderUpdate();
}

void FloorGameFloor::Draw(Camera& camera, const LightMode& lightType) {
	body_.SetLightMode(lightType);
	body_.Draw(camera, kBlendModeNormal);
}

void FloorGameFloor::SwapFloorType(FloorType type) {
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
	}
}

void FloorGameFloor::NormalFloorUpdate() {
	body_.InitWaveData();
	body_.SetColor({ 1.0f - downColor_,1.0f - downColor_,1.0f - downColor_,1.0f });
}

void FloorGameFloor::StickyFloorUpdate() {

	body_.GetWaveData(0).amplitude = 0.1f;
	body_.GetWaveData(0).frequency = 10;
	body_.GetWaveData(0).time += InverseFPS;
	body_.GetWaveData(0).direction = { 1.0f,0.0f,0.0f };

	body_.GetWaveData(1).amplitude = 0.2f;
	body_.GetWaveData(1).frequency = 5;
	body_.GetWaveData(1).time += InverseFPS;
	body_.GetWaveData(1).direction = { 0.0f,0.0f,1.0f };

	float color = autoSwapTimer_ / autoSwapDuration_;
	body_.SetColor({ color - downColor_,color - downColor_,color - downColor_,1.0f });

	if (autoSwapTimer_ > 0.0f) {
		autoSwapTimer_ -= 0.016f;
	} else {
		SwapFloorType(nextFloorType_);
	}
}

void FloorGameFloor::StrongFloorUpdate() {
	body_.InitWaveData();
	body_.SetColor({ 1.0f - downColor_,1.0f - downColor_,1.0f - downColor_,1.0f });
}

void FloorGameFloor::BombFloorUpdate() {
	body_.InitWaveData();
	float color = autoSwapTimer_ / autoSwapDuration_;
	body_.SetColor({ color - downColor_,color - downColor_,color - downColor_,1.0f });

	if (autoSwapTimer_ > 0.0f) {
		autoSwapTimer_ -= 0.016f;
	} else {
		SwapFloorType(nextFloorType_);
		Sound::PlaySE(Sound::kExplosion);
		isExploded_ = true;
	}
}
