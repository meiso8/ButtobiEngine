#include "AmenRa.h"

#include"Window.h"
#include "CollisionConfig.h"
#include"ModelManager.h"
#include"Model.h"
#include"DebugUI.h"
#include"Lerp.h"

AmenRa::AmenRa() {

    object_ = std::make_unique<Object3d>();
    object_->Create();
    object_->SetMesh(ModelManager::GetModel("AmenRa.obj"));

    AABB aabb = { .min = {-1.0f,0.0f,-1.0f},.max = {1.0f,6.01f,1.0f} };

    SetType(kAABB);
    SetCollisionAttribute(kCollisionWall); // ミイラの衝突属性
    SetCollisionMask(kCollisionPlayer | kCollisionEnemy); // プレイヤーや壁と衝突

    // memoのサイズに合わせる
    SetAABB(aabb);
}

void AmenRa::Initialize() {

    object_->Initialize();
    object_->worldTransform_.translate_ = { 0.0f,0.0f,0.0f };
    object_->worldTransform_.rotate_.y = std::numbers::pi_v<float>;
}

void AmenRa::Update() {

    object_->Update();
    DebugUI::CheckObject3d(*object_, "AmenRa");
    ColliderUpdate();
}

void AmenRa::Draw(Camera& camera) {
    object_->Draw(camera);
    ColliderDraw(camera);
}

void AmenRa::OnCollision(Collider* collider) {
    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        // プレイヤーとぶつかったときの処理（必要なら）
    }

}
