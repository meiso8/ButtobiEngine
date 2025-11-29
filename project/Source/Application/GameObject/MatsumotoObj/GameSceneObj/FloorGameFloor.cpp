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

FloorGameFloor::FloorGameFloor() {
	body_.Create();

	models_ =
	{
	  { FloorType::Normal, ModelManager::GetModel(ModelManager::FLOOR)},
	  { FloorType::Sticky, ModelManager::GetModel(ModelManager::MELT_FLOOR) },
	  { FloorType::Strong, ModelManager::GetModel(ModelManager::FLOOR) },
	  { FloorType::Bomb, ModelManager::GetModel(ModelManager::FLOOR) }
	};

	body_.SetMesh(models_[FloorType::Normal]);

	SetCollisionAttribute(kCollisionFloor);
	SetCollisionMask(kCollisionEnemyBomb);
	SetRadius(0.5f);

	isExploded_ = false;
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



	isExploded_ = false;

	body_.InitWaveData();
}

void FloorGameFloor::Update() {
	body_.Update();
	//メッシュをセットする
	body_.SetMesh(models_[floorType_]);
	floorTypeUpdate_[floorType_]();

	ColliderUpdate();
}

void FloorGameFloor::Draw(Camera& camera, const LightMode& lightType) {
	body_.SetLightMode(lightType);
	body_.Draw(camera, kBlendModeNormal);
	ColliderDraw(camera);
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
	body_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
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
	body_.SetColor({ color,color,color,1.0f });

	if (autoSwapTimer_ > 0.0f) {
		autoSwapTimer_ -= 0.016f;
	} else {
		SwapFloorType(nextFloorType_);
	}
}

void FloorGameFloor::StrongFloorUpdate() {
	body_.InitWaveData();
	body_.SetColor({ 0.0f,1.0f,0.0f,1.0f });
}

void FloorGameFloor::BombFloorUpdate() {
	body_.InitWaveData();
	float color = autoSwapTimer_ / autoSwapDuration_;
	body_.SetColor({ color,0.0f,0.0f,1.0f });

	if (autoSwapTimer_ > 0.0f) {
		autoSwapTimer_ -= 0.016f;
	} else {
		SwapFloorType(nextFloorType_);
		isExploded_ = true;
	}
}
