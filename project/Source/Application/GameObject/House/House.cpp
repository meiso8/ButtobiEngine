#include "House.h"
#include"ModelManager.h"
#include"Model.h"
#include"DebugUI.h"
#include"Easing.h"
#include"Input.h"
#include<algorithm>
House::House()
{
    positions_[kHouse_floor].Create();
    positions_[kHouse_wall_L].Create();
    positions_[kHouse_wall_R].Create();
    positions_[kHouse_wall_back].Create();
    positions_[kHouse_door].Create();

    positions_[kHouse_wall_back].worldTransform_.Parent(positions_[kHouse_floor].worldTransform_);
    positions_[kHouse_wall_L].worldTransform_.Parent(positions_[kHouse_floor].worldTransform_);
    positions_[kHouse_wall_R].worldTransform_.Parent(positions_[kHouse_floor].worldTransform_);
    positions_[kHouse_door].worldTransform_.Parent(positions_[kHouse_wall_back].worldTransform_);

    positions_[kHouse_floor].SetMesh(ModelManager::GetModel(ModelManager::HOUSE_FLOOR));
    positions_[kHouse_wall_L].SetMesh(ModelManager::GetModel(ModelManager::HOUSE_WALL_L));
    positions_[kHouse_wall_R].SetMesh(ModelManager::GetModel(ModelManager::HOUSE_WALL_R));
    positions_[kHouse_wall_back].SetMesh(ModelManager::GetModel(ModelManager::HOUSE_WALL_BACK));
    positions_[kHouse_door].SetMesh(ModelManager::GetModel(ModelManager::HOUSE_DOOR));
}

void House::Initialize()
{
    positions_[kHouse_floor].Initialize();
    positions_[kHouse_wall_L].Initialize();
    positions_[kHouse_wall_R].Initialize();
    positions_[kHouse_wall_back].Initialize();
    positions_[kHouse_door].Initialize();

    positions_[kHouse_floor].worldTransform_.scale_ = { 2.5f,2.5f,2.5f };
    positions_[kHouse_floor].worldTransform_.translate_.y = -0.45f;
    positions_[kHouse_wall_back].worldTransform_.translate_.z = 2.0f;
    positions_[kHouse_wall_L].worldTransform_.translate_.x = -2.0f;
    positions_[kHouse_wall_R].worldTransform_.translate_.x = 2.0f;
    positions_[kHouse_door].worldTransform_.translate_ = { -0.98f,0.76f,0.0f };

    isDoorOpen_ = false;
    isWallBrake_ = false;
    wallBreakTimer_ = 0.0f;
}

void House::Update()
{
    if (*hitCount_ > 0) {
        isDoorOpen_ = true;
    }

    if (*hitCount_ >= maxHitCount_ - 1) {
        isWallBrake_ = true;
    }

    if (isWallBrake_) {

        wallBreakTimer_ += InverseFPS * 0.5f;
        wallBreakTimer_ = std::clamp(wallBreakTimer_, 0.0f, 1.0f);

        positions_[kHouse_wall_L].worldTransform_.rotate_.z = Easing::EaseInOutBounce(0.0f, std::numbers::pi_v<float>*0.5f, wallBreakTimer_);
        positions_[kHouse_wall_R].worldTransform_.rotate_.z = Easing::EaseInOutBounce(0.0f, -std::numbers::pi_v<float>*0.5f, wallBreakTimer_);
        positions_[kHouse_wall_back].worldTransform_.rotate_.x = Easing::EaseInOutBounce(0.0f, std::numbers::pi_v<float>*0.5f, wallBreakTimer_);
       
    } else {
        if (isDoorOpen_) {
            positions_[kHouse_door].worldTransform_.rotate_.y = Lerp(positions_[kHouse_door].worldTransform_.rotate_.y, -std::numbers::pi_v<float>*0.5f, 0.5f);
        }
    }

    positions_[kHouse_floor].Update();
    positions_[kHouse_wall_L].Update();
    positions_[kHouse_wall_R].Update();
    positions_[kHouse_wall_back].Update();
    positions_[kHouse_door].Update();

    DebugUI::CheckObject3d(positions_[kHouse_floor], "kHouse_floor");
    DebugUI::CheckObject3d(positions_[kHouse_wall_L], "kHouse_wall_L");
    DebugUI::CheckObject3d(positions_[kHouse_wall_R], "kHouse_wall_R");
    DebugUI::CheckObject3d(positions_[kHouse_wall_back], "kHouse_wall_back");
    DebugUI::CheckObject3d(positions_[kHouse_door], "kHouse_door");
}

void House::Draw(Camera& camera, const LightMode& lightType)
{
    positions_[kHouse_floor].SetLightMode(lightType);
    positions_[kHouse_wall_L].SetLightMode(lightType);
    positions_[kHouse_wall_R].SetLightMode(lightType);
    positions_[kHouse_wall_back].SetLightMode(lightType);
    positions_[kHouse_door].SetLightMode(lightType);

    positions_[kHouse_floor].Draw(camera);
    positions_[kHouse_wall_L].Draw(camera);
    positions_[kHouse_wall_R].Draw(camera);
    positions_[kHouse_wall_back].Draw(camera);
    positions_[kHouse_door].Draw(camera);
}
