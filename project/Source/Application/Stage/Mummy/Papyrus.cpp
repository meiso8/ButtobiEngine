#include "Papyrus.h"

#include "CollisionConfig.h"
#include"ModelManager.h"
#include"Model.h"
#include"JsonFile.h"
#include"DebugUI.h"
Papyrus::Papyrus() {

    object_ = std::make_unique<Object3d>();
    object_->Create();
    object_->SetMesh(ModelManager::GetModel(ModelManager::PAPYRUS));

    Json file = JsonFile::GetJsonFiles("memo");
    std::string sizeKeys = "bookSize";

    AABB aabb; // AABBの読み込み 
    aabb.min.x = file[sizeKeys]["min"]["x"];
    aabb.min.y = file[sizeKeys]["min"]["y"];
    aabb.min.z = file[sizeKeys]["min"]["z"];

    aabb.max.x = file[sizeKeys]["max"]["x"];
    aabb.max.y = file[sizeKeys]["max"]["y"];
    aabb.max.z = file[sizeKeys]["max"]["z"];

    SetType(kAABB);
    SetCollisionAttribute(kCollisionWall); // ミイラの衝突属性
    SetCollisionMask(kCollisionPlayer | kCollisionEnemy); // プレイヤーや壁と衝突

    // memoのサイズに合わせる
    SetAABB(aabb);
}

void Papyrus::Initialize() {
    object_->Initialize();
    object_->worldTransform_.translate_ = { 0.0f,2.0f,5.0f };
}

void Papyrus::Update() {
    object_->Update();
    DebugUI::CheckObject3d(*object_,"Papyrus");
}

void Papyrus::Draw(Camera& camera) {
    object_->Draw(camera);

}

void Papyrus::OnCollision(Collider* collider) {
    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        // プレイヤーとぶつかったときの処理（必要なら）
    }

}
