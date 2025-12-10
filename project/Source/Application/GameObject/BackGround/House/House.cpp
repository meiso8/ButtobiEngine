#include "House.h"
#include"ModelManager.h"
#include"Model.h"
#include"DebugUI.h"
#include"Easing.h"
#include"Input.h"
#include<algorithm>
#include"CollisionConfig.h"
#include"Sound.h"
House::House()
{

    floorPos_.Create();
    floorPos_.SetMesh(ModelManager::GetModel(ModelManager::HOUSE_FLOOR));

    doorPos_.Create();
    doorPos_.SetMesh(ModelManager::GetModel(ModelManager::HOUSE_DOOR));


    wallBackPos_.Create();
    wallBackPos_.SetMesh(ModelManager::GetModel(ModelManager::HOUSE_WALL_BACK));

    colliders_[kHouse_wall_L] = std::make_unique<HouseCollider>();
    colliders_[kHouse_wall_R] = std::make_unique<HouseCollider>();
    colliders_[kHouse_wall_back_L] = std::make_unique<HouseCollider>();
    colliders_[kHouse_wall_back_R] = std::make_unique<HouseCollider>();
    colliders_[kHouse_door] = std::make_unique<HouseCollider>();

    colliders_[kHouse_wall_L]->SettingAABB({ .min = {-0.25f,0.0f,-6.0f},.max = {0.25f,12.0f,6.0f} }, ModelManager::GetModel(ModelManager::HOUSE_WALL_L));
    colliders_[kHouse_wall_R]->SettingAABB({ .min = {-0.25f,0.0f,-6.0f},.max = {0.25f,12.0f,6.0f} }, ModelManager::GetModel(ModelManager::HOUSE_WALL_R));
    colliders_[kHouse_wall_back_L]->SettingAABB({ .min = {-3.0f,0.0f,-0.25f},.max = {3.0f,12.0f,0.25f} },nullptr);
    colliders_[kHouse_wall_back_R]->SettingAABB({ .min = {-3.0f,0.0f,-0.25f},.max = {3.0f,12.0f,0.25f} }, nullptr);
    colliders_[kHouse_door]->SettingSphere(2.5f,nullptr);

    colliders_[kHouse_wall_L]->pos_.worldTransform_.Parent(floorPos_.worldTransform_);
    colliders_[kHouse_wall_R]->pos_.worldTransform_.Parent(floorPos_.worldTransform_);

    wallBackPos_.worldTransform_.Parent(floorPos_.worldTransform_);

    colliders_[kHouse_wall_back_L]->pos_.worldTransform_.Parent(wallBackPos_.worldTransform_);
    colliders_[kHouse_wall_back_R]->pos_.worldTransform_.Parent(wallBackPos_.worldTransform_);

    doorPos_.worldTransform_.Parent(wallBackPos_.worldTransform_);

    colliders_[kHouse_door]->pos_.worldTransform_.Parent(doorPos_.worldTransform_);



}

void House::Initialize()
{
    floorPos_.Initialize();
    floorPos_.worldTransform_.scale_ = { 4.0f,4.0f,4.0f };
    floorPos_.worldTransform_.translate_.y = -1.0f;

    doorPos_.Initialize();
    doorPos_.worldTransform_.translate_ = { -0.98f,0.76f,0.0f };

    wallBackPos_.Initialize();
    wallBackPos_.worldTransform_.translate_.z = 2.0f;

    colliders_[kHouse_wall_L]->Initialize();
    colliders_[kHouse_wall_R]->Initialize();
    colliders_[kHouse_wall_back_L]->Initialize();
    colliders_[kHouse_wall_back_R]->Initialize();
    colliders_[kHouse_door]->Initialize();

    colliders_[kHouse_wall_back_L]->pos_.worldTransform_.translate_.x = 1.5f;
    colliders_[kHouse_wall_back_R]->pos_.worldTransform_.translate_.x = -1.5f;

    colliders_[kHouse_wall_L]->pos_.worldTransform_.translate_.x = -2.0f;
    colliders_[kHouse_wall_R]->pos_.worldTransform_.translate_.x = 2.0f;
    colliders_[kHouse_door]->pos_.worldTransform_.translate_ = { 0.98f,0.0f,0.0f };

    isDoorOpen_ = false;
    isWallBrake_ = false;
    wallBreakTimer_ = 0.0f;
}

void House::Update()
{
    if (colliders_[kHouse_door]->isHit_) {
        isDoorOpen_ = true;
    }

    if (isDoorOpen_) {
        doorPos_.worldTransform_.rotate_.y = Lerp(doorPos_.worldTransform_.rotate_.y, -std::numbers::pi_v<float>*0.5f, 0.5f);
    }

    if (*hitCount_ >= maxHitCount_ - 1) {
        isWallBrake_ = true;
    }

    if (isWallBrake_) {

        wallBreakTimer_ += InverseFPS * 0.5f;
        wallBreakTimer_ = std::clamp(wallBreakTimer_, 0.0f, 1.0f);

        colliders_[kHouse_wall_L]->pos_.worldTransform_.rotate_.z = Easing::EaseInOutBounce(0.0f, std::numbers::pi_v<float>*0.5f, wallBreakTimer_);
        colliders_[kHouse_wall_R]->pos_.worldTransform_.rotate_.z = Easing::EaseInOutBounce(0.0f, -std::numbers::pi_v<float>*0.5f, wallBreakTimer_);
       wallBackPos_.worldTransform_.rotate_.x = Easing::EaseInOutBounce(0.0f, std::numbers::pi_v<float>*0.5f, wallBreakTimer_);
    } 

    floorPos_.Update();
    doorPos_.Update();
    wallBackPos_.Update();
    colliders_[kHouse_wall_L]->Update();
    colliders_[kHouse_wall_R]->Update();
    colliders_[kHouse_wall_back_L]->Update();
    colliders_[kHouse_wall_back_R]->Update();
    colliders_[kHouse_door]->Update();

    DebugUI::CheckObject3d(floorPos_, "kHouse_floor");
    DebugUI::CheckObject3d(doorPos_, "doorPos");
    DebugUI::CheckObject3d(colliders_[kHouse_wall_L]->pos_, "kHouse_wall_L");
    DebugUI::CheckObject3d(colliders_[kHouse_wall_R]->pos_, "kHouse_wall_R");
    DebugUI::CheckObject3d(colliders_[kHouse_wall_back_L]->pos_, "kHouse_wall_back_L");
    DebugUI::CheckObject3d(colliders_[kHouse_wall_back_R]->pos_, "kHouse_wall_back_R");
    DebugUI::CheckObject3d(colliders_[kHouse_door]->pos_, "kHouse_door");
}

void House::Draw(Camera& camera, const LightMode& lightType)
{
    floorPos_.Draw(camera);
    doorPos_.Draw(camera);
    wallBackPos_.Draw(camera);
    colliders_[kHouse_wall_L]->Draw(camera);
    colliders_[kHouse_wall_R]->Draw(camera);
    colliders_[kHouse_wall_back_L]->Draw(camera);
    colliders_[kHouse_wall_back_R]->Draw(camera);
    colliders_[kHouse_door]->Draw(camera);

}

HouseCollider::HouseCollider()
{
    SetCollisionAttribute(kCollisionHouse);

    SetCollisionMask(kCollisionPlayerBullet);

    pos_.Create();
}

void HouseCollider::SettingAABB(const AABB& aabb, Model* model)
{
    SetType(Collider::ColliderType::kAABB);
    SetAABB(aabb);
    pos_.SetMesh(model);
}

void HouseCollider::SettingSphere(const float& radius, Model* model)
{
    SetType(Collider::ColliderType::kSphere);
    SetRadius(radius);
    pos_.SetMesh(model);
}

void HouseCollider::Initialize()
{
    isHit_ = false;
    pos_.Initialize();
}

void HouseCollider::Update()
{
    isHit_ = false;
    pos_.Update();
    ColliderUpdate();
}

void HouseCollider::Draw(Camera& camera)
{

    pos_.SetLightMode(LightMode::kLightModeHalfL);
    pos_.Draw(camera);

    ColliderDraw(camera);

}

void HouseCollider::OnCollision(Collider* collider)
{
    isHit_ = true;
    OnCollisionCollider();
    Sound::PlaySE(Sound::kDefeatBoss);
}

Vector3 HouseCollider::GetWorldPosition() const
{
    return pos_.worldTransform_.GetWorldPosition();
}
