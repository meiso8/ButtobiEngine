#include "Water.h"
#include"Window.h"

#include "CollisionConfig.h"
#include"ModelManager.h"
#include"Model.h"
#include"JsonFile.h"
#include"DebugUI.h"

Water::Water() {

    object_ = std::make_unique<Object3d>();
    object_->Create();
    object_->SetMesh(ModelManager::GetModel(ModelManager::WATER));

    object_->GetWaveData(0).amplitude = 0.3f;
    object_->GetWaveData(0).direction = Normalize(Vector3{ 0.134f,0.12f,0.98f });
    object_->GetWaveData(0).frequency = 2.0f;
    object_->GetWaveData(1).amplitude = 0.2f;
    object_->GetWaveData(1).direction = { 1.0f,0.0f,0.0f };
    object_->GetWaveData(1).frequency = 4.0f;

    AABB aabb = { .min = {-12.5f,-0.5f,-12.5f},.max = {12.5f,0.5f,12.5f} };
    SetType(kAABB);
    SetCollisionAttribute(kCollisionWall); // ミイラの衝突属性
    SetCollisionMask(kCollisionPlayer | kCollisionEnemy); // プレイヤーや壁と衝突

    // memoのサイズに合わせる
    SetAABB(aabb);
}

void Water::Initialize() {
    object_->GetWaveData(0).time = 0.0f;
    object_->GetWaveData(1).time = 0.0f;
    object_->Initialize();
    object_->worldTransform_.translate_ = { 0.0f,1.0f,0.0f };

}

void Water::Update() {

    object_->GetWaveData(0).time += InverseFPS;
    object_->GetWaveData(1).time = object_->GetWaveData(0).time + 1.5f;
    object_->Update();
    DebugUI::CheckObject3d(*object_, "Water");
}

void Water::Draw(Camera& camera) {
    object_->Draw(camera,kBlendModeMultiply);

}

void Water::OnCollision(Collider* collider) {
    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        // プレイヤーとぶつかったときの処理（必要なら）
    }

}
