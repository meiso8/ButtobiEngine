#include "FloorGameFloor.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include"CubeMesh.h"
#include "Input.h"

FloorGameFloor::FloorGameFloor() {
    body_.Create();

    models_ =
    {
      { FloorType::Normal, ModelManager::GetModel(ModelManager::FLOOR)},
      { FloorType::Sticky, ModelManager::GetModel(ModelManager::MELT_FLOOR) },
      //仮にプレイヤーの腕を入れているだけなので後で変更してください吉田
      { FloorType::Strong, ModelManager::GetModel(ModelManager::PLAYER_ARM_L) }
    };

    //cubeMesh_ = std::make_unique<CubeMesh>();
    //cubeMesh_.get()->Create(Texture::WHITE_1X1);
    //cubeMesh_->SetColor({ 0.5f,0.5f,0.5f,1.0f });

    body_.SetMesh(models_[FloorType::Normal]);
}

FloorGameFloor::~FloorGameFloor() {
}

void FloorGameFloor::Initialize() {
    body_.Initialize();
    autoSwapTimer_ = 0.0f;
    floorType_ = FloorType::Normal;
    autoSwapDuration_ = 3.0f;
    nextFloorType_ = FloorType::Sticky;
    floorTypeUpdate_ = {
        {FloorType::Normal, std::bind(&FloorGameFloor::NormalFloorUpdate, this)},
        {FloorType::Sticky, std::bind(&FloorGameFloor::StickyFloorUpdate, this)},
        {FloorType::Strong, std::bind(&FloorGameFloor::StrongFloorUpdate, this)},
    };
}

void FloorGameFloor::Update() {
    body_.Update();
    //メッシュをセットする
    body_.SetMesh(models_[floorType_]);
    floorTypeUpdate_[floorType_]();
}

void FloorGameFloor::Draw(Camera& camera, const LightMode& lightType) {
    body_.SetLightMode(lightType);
    body_.Draw(camera, kBlendModeNormal);
}

void FloorGameFloor::SwapFloorType(FloorType type) {
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
    default:
        break;
    }
}

void FloorGameFloor::SwapNextFloorType() {
    SwapFloorType(nextFloorType_);
}

void FloorGameFloor::NormalFloorUpdate() {
    models_[floorType_]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
}

void FloorGameFloor::StickyFloorUpdate() {

    models_[floorType_]->SetColor({ 0.0f,0.0f,(autoSwapTimer_ / autoSwapDuration_),1.0f });

    if (autoSwapTimer_ > 0.0f) {
        autoSwapTimer_ -= 0.016f;
    } else {
        SwapFloorType(nextFloorType_);
    }
}

void FloorGameFloor::StrongFloorUpdate() {
    models_[floorType_]->SetColor({ 0.0f,1.0f,0.0f,1.0f });
}