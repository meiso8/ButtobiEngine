#include "PuzzleObj.h"

#include"Window.h"
#include "CollisionConfig.h"
#include"ModelManager.h"
#include"Model.h"
#include"DebugUI.h"
#include"Lerp.h"

PuzzleObj::PuzzleObj() {

    object_ = std::make_unique<Object3d>();
    object_->Create();
    object_->SetMesh(ModelManager::GetModel(ModelManager::BOX));

    AABB aabb = { .min = {-0.5f,-0.5f,-0.5f},.max = {0.5f,0.5f,0.5f} };

    SetType(kAABB);
    SetCollisionAttribute(kCollisionWall); // ミイラの衝突属性
    SetCollisionMask(kCollisionPlayer | kCollisionEnemy); // プレイヤーや壁と衝突

    // memoのサイズに合わせる
    SetAABB(aabb);
}

void PuzzleObj::Initialize() {

    object_->Initialize();
    object_->worldTransform_.translate_ = { 0.0f,0.0f,0.0f };

}

void PuzzleObj::Update() {

    object_->Update();
    object_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
    DebugUI::CheckObject3d(*object_, "PuzzleObj");
}

void PuzzleObj::Draw(Camera& camera) {
    object_->Draw(camera);

}

void PuzzleObj::OnCollision(Collider* collider) {
    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        // プレイヤーとぶつかったときの処理（必要なら）
    }

}
