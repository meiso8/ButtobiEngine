#include"StrippedFloor.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"FloorGameFloor.h"
#include <algorithm>
#include <memory>

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

	isActive_ = false;
}

StrippedFloor::~StrippedFloor() {
}

void StrippedFloor::Initialize() {
	body_.Initialize();
}

void StrippedFloor::Update() {
	if (!isActive_) { return; }
	
	if (type_ != requestType_) {
		type_ = requestType_;
		body_.SetMesh(models_[type_]);
	}

	body_.Update();
}

void StrippedFloor::Draw(Camera& camera, const LightMode& lightType) {
	if (!isActive_) { return; }

	body_.SetLightMode(lightType);
	body_.Draw(camera, kBlendModeNormal);
}
