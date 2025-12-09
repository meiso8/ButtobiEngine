#include "House.h"
#include"ModelManager.h"
#include"Model.h"
#include"DebugUI.h"
#include"Easing.h"
#include"Input.h"
#include<algorithm>
#include"CollisionConfig.h"
House::House()
{

    colliders_[kHouse_floor] = std::make_unique<HouseCollider>();
    colliders_[kHouse_wall_L] = std::make_unique<HouseCollider>();
    colliders_[kHouse_wall_R] = std::make_unique<HouseCollider>();
    colliders_[kHouse_wall_back] = std::make_unique<HouseCollider>();
    colliders_[kHouse_door] = std::make_unique<HouseCollider>();

    colliders_[kHouse_floor]->Setting({ .min = {-6.0f,0.0f,-6.0f},.max = {6.0f,0.2f,6.0f} }, ModelManager::GetModel(ModelManager::HOUSE_FLOOR));
    colliders_[kHouse_wall_L]->Setting({ .min = {-0.25f,0.0f,-6.0f},.max = {0.25f,12.0f,6.0f} }, ModelManager::GetModel(ModelManager::HOUSE_WALL_L));
    colliders_[kHouse_wall_R]->Setting({ .min = {-0.25f,0.0f,-6.0f},.max = {0.25f,12.0f,6.0f} }, ModelManager::GetModel(ModelManager::HOUSE_WALL_R));
    colliders_[kHouse_wall_back]->Setting({ .min = {-6.0f,0.0f,-0.25f},.max = {6.0f,12.0f,0.25f} }, ModelManager::GetModel(ModelManager::HOUSE_WALL_BACK));
    colliders_[kHouse_door]->Setting({ .min = {0.0f,0.0f,-0.25f},.max = {6.0f,6.0f,0.25f} }, ModelManager::GetModel(ModelManager::HOUSE_DOOR));

    colliders_[kHouse_wall_back]->pos_.worldTransform_.Parent(colliders_[kHouse_floor]->pos_.worldTransform_);
    colliders_[kHouse_wall_L]->pos_.worldTransform_.Parent(colliders_[kHouse_floor]->pos_.worldTransform_);
    colliders_[kHouse_wall_R]->pos_.worldTransform_.Parent(colliders_[kHouse_floor]->pos_.worldTransform_);
    colliders_[kHouse_door]->pos_.worldTransform_.Parent(colliders_[kHouse_wall_back]->pos_.worldTransform_);

    //positions_[kHouse_floor].SetMesh();
    //positions_[kHouse_wall_L].SetMesh();
    //positions_[kHouse_wall_R].SetMesh();
    //positions_[kHouse_wall_back].SetMesh();
    //positions_[kHouse_door].SetMesh(ModelManager::GetModel(ModelManager::HOUSE_DOOR));
}

void House::Initialize()
{
    colliders_[kHouse_floor]->Initialize();
    colliders_[kHouse_wall_L]->Initialize();
    colliders_[kHouse_wall_R]->Initialize();
    colliders_[kHouse_wall_back]->Initialize();
    colliders_[kHouse_door]->Initialize();

    colliders_[kHouse_floor]->pos_.worldTransform_.scale_ = { 4.0f,4.0f,4.0f };
    colliders_[kHouse_floor]->pos_.worldTransform_.translate_.y = -1.0f;
    colliders_[kHouse_wall_back]->pos_.worldTransform_.translate_.z = 2.0f;
    colliders_[kHouse_wall_L]->pos_.worldTransform_.translate_.x = -2.0f;
    colliders_[kHouse_wall_R]->pos_.worldTransform_.translate_.x = 2.0f;
    colliders_[kHouse_door]->pos_.worldTransform_.translate_ = { -0.98f,0.76f,0.0f };

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

        colliders_[kHouse_wall_L]->pos_.worldTransform_.rotate_.z = Easing::EaseInOutBounce(0.0f, std::numbers::pi_v<float>*0.5f, wallBreakTimer_);
        colliders_[kHouse_wall_R]->pos_.worldTransform_.rotate_.z = Easing::EaseInOutBounce(0.0f, -std::numbers::pi_v<float>*0.5f, wallBreakTimer_);
        colliders_[kHouse_wall_back]->pos_.worldTransform_.rotate_.x = Easing::EaseInOutBounce(0.0f, std::numbers::pi_v<float>*0.5f, wallBreakTimer_);

    } else {
        if (isDoorOpen_) {
            colliders_[kHouse_door]->pos_.worldTransform_.rotate_.y = Lerp(colliders_[kHouse_door]->pos_.worldTransform_.rotate_.y, -std::numbers::pi_v<float>*0.5f, 0.5f);
        }
    }

    colliders_[kHouse_floor]->Update();
    colliders_[kHouse_wall_L]->Update();
    colliders_[kHouse_wall_R]->Update();
    colliders_[kHouse_wall_back]->Update();
    colliders_[kHouse_door]->Update();

    DebugUI::CheckObject3d(colliders_[kHouse_floor]->pos_, "kHouse_floor");
    DebugUI::CheckObject3d(colliders_[kHouse_wall_L]->pos_, "kHouse_wall_L");
    DebugUI::CheckObject3d(colliders_[kHouse_wall_R]->pos_, "kHouse_wall_R");
    DebugUI::CheckObject3d(colliders_[kHouse_wall_back]->pos_, "kHouse_wall_back");
    DebugUI::CheckObject3d(colliders_[kHouse_door]->pos_, "kHouse_door");
}

void House::Draw(Camera& camera, const LightMode& lightType)
{
    colliders_[kHouse_floor]->Draw(camera);
    colliders_[kHouse_wall_L]->Draw(camera);
    colliders_[kHouse_wall_R]->Draw(camera);
    colliders_[kHouse_wall_back]->Draw(camera);
    colliders_[kHouse_door]->Draw(camera);
}

HouseCollider::HouseCollider()
{

    SetType(Collider::ColliderType::kAABB);

    SetCollisionAttribute(kCollisionHouse);

    SetCollisionMask(kCollisionPlayerBullet);

    pos_.Create();
}

void HouseCollider::Setting(AABB aabb, Model* model)
{
    SetAABB(aabb);
    pos_.SetMesh(model);
}

void HouseCollider::Initialize()
{
    pos_.Initialize();
}

void HouseCollider::Update()
{
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
    OnCollisionCollider();
}

Vector3 HouseCollider::GetWorldPosition() const
{
    return pos_.worldTransform_.GetWorldPosition();
}
