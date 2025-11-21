#include "FloorBullet.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>

#include"CubeMesh.h"
#include"CollisionConfig.h"

#include"Data/FloorData.h"


FloorBullet::FloorBullet() {
    body_.Create();
    body_.SetColor({ 1.0f,0.0f,0.0f,1.0f });
    body_.SetMesh(cubeMesh_.get());

    models_ =
    {
      { FloorType::Normal, ModelManager::GetModel(ModelManager::FLOOR)},
      { FloorType::Sticky, ModelManager::GetModel(ModelManager::MELT_FLOOR) },
      { FloorType::Strong, ModelManager::GetModel(ModelManager::FLOOR) }
    };

    body_.SetMesh(models_[FloorType::Normal]);


    SetRadius(0.5f);
    SetCollisionAttribute(kCollisionPlayerBullet);
    // 弾は「敵と敵の弾」とだけ衝突したい
    SetCollisionMask(kCollisionEnemy | kCollisionEnemyBullet);
}

FloorBullet::~FloorBullet() {
}

void FloorBullet::Initialize() {
    body_.Initialize();
    moveDir_ = { 0.0f,0.0f,1.0f };
    moveSpeed_ = 0.2f;
    lifeTimer_ = 0.0f;
    lifeDuration_ = 2.0f;
    isActive_ = false;
    isHit_ = false;
    size_ = 1.0f;

    floorType_ = FloorType::Normal;
    reqestFloorType_ = FloorType::Normal;
}
void FloorBullet::InitFlagAndPosAndTimer()
{
    lifeTimer_ = 0.0f;
    body_.worldTransform_.translate_ = { 0.0f,-10.0f,0.0f };
    isActive_ = false;
}
void FloorBullet::OnCollision(Collider* collider)
{


    if (!isActive_) {
        return;
    }

    if (collider->GetCollisionAttribute() == kCollisionEnemy || collider->GetCollisionAttribute() == kCollisionEnemyBullet) {
        if (!isHit_) {
            isHit_ = true;
        }
    }
    //デバック用
    OnCollisionCollider();

}
Vector3 FloorBullet::GetWorldPosition() const
{
    return body_.worldTransform_.GetWorldPosition();
}
void FloorBullet::Update() {

#ifdef USE_IMGUI
    DebugUI::CheckFlag(isActive_, "isActive");
    DebugUI::CheckFlag(isHit_, "isHit_");
    ImGui::Text(" lifeTimer_ %f", lifeTimer_);
    DebugUI::CheckWorldTransform(body_.worldTransform_, "BulletTransform");
#endif

    if (!isActive_) {
        return;
    }

    if (isHit_) {
        InitFlagAndPosAndTimer();
        isHit_ = false;
    }

    Move();
    body_.Update();
    ColliderUpdate();

}

void FloorBullet::Draw(Camera& camera, const LightMode& lightType) {
  
    ColliderDraw(camera);

    if (!isActive_) {
        return;
    }

    if (floorType_ != reqestFloorType_)
    {
        floorType_ = reqestFloorType_;
        body_.SetMesh(models_[floorType_]);
    }

    body_.SetLightMode(lightType);
    body_.Draw(camera, kBlendModeNormal);


}

void FloorBullet::Shot(const Vector3& position, const Vector3& direction, const float& speed, const float& size, FloorType type) {
    body_.worldTransform_.translate_ = position;
    moveDir_ = Normalize(direction);
    moveSpeed_ = speed;
    size_ = size;
    lifeTimer_ = lifeDuration_;
    isActive_ = true;
    isHit_ = false;
    reqestFloorType_ = type;
}


void FloorBullet::Move()
{
    if (lifeTimer_ <= 0.0f) {
        InitFlagAndPosAndTimer();
        return;
    } else {
        lifeTimer_ -= 0.016f;
    }

    body_.worldTransform_.rotate_.y = atan2f(moveDir_.x, moveDir_.z);
    body_.worldTransform_.translate_ += moveDir_ * moveSpeed_;

}
