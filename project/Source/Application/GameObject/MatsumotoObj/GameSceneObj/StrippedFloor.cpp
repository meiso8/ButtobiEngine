#include"StrippedFloor.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"FloorGameFloor.h"
#include <algorithm>
#include <memory>
#include "MatsumotoObj/GameSceneObj/Data/MapData.h"

StrippedFloor::StrippedFloor() {
	body_.Create();
	models_ =
	{
	  { FloorType::Normal, ModelManager::GetModel(ModelManager::FLOOR)},
	  { FloorType::Sticky, ModelManager::GetModel(ModelManager::MELT_FLOOR) },
	  { FloorType::Strong, ModelManager::GetModel(ModelManager::HARD_FLOOR) },
	  { FloorType::Bomb, ModelManager::GetModel(ModelManager::EXPLOTION_FLOOR)}
	};
	type_ = FloorType::Normal;
	requestType_ = FloorType::Normal;
	body_.SetMesh(models_[type_]);
	body_.SetColor({ 1.0f,1.0f,1.0f,0.7f });

	isActive_ = false;

	body_.worldTransform_.scale_ = { kHalfFloorSize * 2.0f,kHalfFloorSize * 2.0f,kHalfFloorSize * 2.0f };
}

StrippedFloor::~StrippedFloor() {
}

void StrippedFloor::Initialize() {
	body_.Initialize();
	body_.worldTransform_.scale_ = { kHalfFloorSize * 2.0f,kHalfFloorSize * 2.0f,kHalfFloorSize * 2.0f };
}

void StrippedFloor::Update() {
	if (!isActive_) { return; }
	
	if (type_ != requestType_) {
		type_ = requestType_;
		body_.SetMesh(models_[type_]);
	}

	body_.Update();
}

void StrippedFloor::Draw(Camera& camera) {
	if (!isActive_) { return; }

	body_.Draw(camera, kBlendModeNormal);
}
