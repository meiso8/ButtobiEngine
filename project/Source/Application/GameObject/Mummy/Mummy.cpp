#include "Mummy.h"
#include "CollisionConfig.h"
#include"ModelManager.h"
#include"Model.h"

Mummy::Mummy() {
    object_ = std::make_unique<Object3d>();
    object_->Create();
    object_->SetMesh(ModelManager::GetModel(ModelManager::MUMMY));

    coffinModel_ = ModelManager::GetModel(ModelManager::COFFIN_GLTF);
    aniObj_ = std::make_unique<AnimationObject3d>(); 
    aniObj_->Create();

    skinningModel_ = std::make_unique<SkinningModel>();
    skinningModel_->CreateDatas(coffinModel_, coffinModel_);
    aniObj_->SetMeshAndData(skinningModel_.get());


    SetType(kAABB); // 必要に応じて kSphere に変更してもOK！

    SetCollisionAttribute(kCollisionMummy); // ミイラの衝突属性
    SetCollisionMask(kCollisionPlayer | kCollisionEnemy); // プレイヤーや壁と衝突

    // ミイラのサイズに合わせてAABBを設定（仮のサイズ）
    SetAABB({ {-0.5f, 0.0f, -0.5f}, {0.5f, 1.5f, 0.5f} });
}

void Mummy::Initialize() {
    aniObj_->Initialize();
    object_->Initialize();
}

void Mummy::Update() {
    aniObj_->Update();
    object_->Update();
    ColliderUpdate();
}

void Mummy::Draw(Camera& camera) {
    aniObj_->Draw(camera);
    object_->Draw(camera);
    ColliderDraw(camera);
}

void Mummy::OnCollision(Collider* collider) {
    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        // プレイヤーとぶつかったときの処理（必要なら）
    }
    OnCollisionCollider();
}
