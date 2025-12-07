#include "EnemyBeaK.h"
#include"CollisionConfig.h"
#include"Camera.h"
#include"DebugUI.h"
EnemyBeak::EnemyBeak()
{
    //コライダーの設定
    SetRadius(0.5f);
    SetCollisionAttribute(kCollisionEnemy);
    // 敵は「プレイヤー」と「プレイヤーの弾」と衝突したい
    SetCollisionMask(kCollisionPlayer | kCollisionFloor);

}

void EnemyBeak::Initialize()
{

    isFloorHit_ = false;

    beak_.Initialize();
    beak_.translate_.y = 1.2f;
    beak_.translate_.z = 1.5f;
}

void EnemyBeak::Update()
{
    isFloorHit_ = false;
    
#ifdef USE_IMGUI
    DebugUI::CheckWorldTransform(beak_, "EnemyBeak");
    ImGui::Text("GetWorldPos %f %f %f", GetWorldPosition());
    ImGui::Checkbox("isFloorHit_", &isFloorHit_);
    WorldTransformUpdate(beak_);
    ColliderUpdate();
#endif
}

void EnemyBeak::OnCollision(Collider* collider)
{

    if (collider->GetCollisionAttribute() == kCollisionFloor) {
        OnCollisionCollider();
        isFloorHit_ = true;
    }
}

void EnemyBeak::Draw(Camera& camera)
{
    ColliderDraw(camera);
}

Vector3 EnemyBeak::GetWorldPosition() const
{
    return beak_.GetWorldPosition();
}
