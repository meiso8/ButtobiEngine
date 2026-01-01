#include "Mummy.h"
#include "CollisionConfig.h"
#include"ModelManager.h"
#include"Model.h"

Mummy::Mummy() {
    platform_ = std::make_unique<Platform>();

    object_ = std::make_unique<Object3d>();
    object_->Create();
    object_->SetMesh(ModelManager::GetModel(ModelManager::MUMMY));

    coffinModel_ = ModelManager::GetModel(ModelManager::COFFIN_GLTF);
    aniObj_ = std::make_unique<AnimationObject3d>();
    aniObj_->Create();

    //ペアレント
    object_->worldTransform_.Parent(platform_->GetWorldTransform());
    aniObj_->worldTransform_.Parent(platform_->GetWorldTransform());

    skinningModel_ = std::make_unique<SkinningModel>();
    skinningModel_->CreateDatas(coffinModel_, coffinModel_);
    aniObj_->SetMeshAndData(skinningModel_.get());

    SetType(kAABB);
    SetCollisionAttribute(kCollisionMummy); // ミイラの衝突属性
    SetCollisionMask(kCollisionPlayer | kCollisionEnemy); // プレイヤーや壁と衝突

    // ミイラのサイズに合わせてAABBを設定（仮のサイズ）
    SetAABB({ {-0.25f, 0.0f, -1.0f}, {0.25f, 0.5f, 1.0f} });
}

void Mummy::Initialize() {
    isOpen_ = false;
    platform_->Initialize();
    aniObj_->Initialize();
    object_->Initialize();
    // 台の高さを取得してオフセット 
    float platformHeight = platform_->GetAABB().max.y-platform_->GetAABB().min.y;
    object_->worldTransform_.translate_.y = platformHeight;
    aniObj_->worldTransform_.translate_.y = platformHeight;
}

void Mummy::Update() {

    if (isOpen_) {
        //ループしない
        aniObj_->UpdateAniTimer(false);
    }
    platform_->Update();
    aniObj_->Update();
    object_->Update();

}

void Mummy::Draw(Camera& camera) {
    platform_->Draw(camera);
    aniObj_->Draw(camera, kBlendModeNormal, kCullModeNone);
    object_->Draw(camera);

}

void Mummy::OnCollision(Collider* collider) {
    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        // プレイヤーとぶつかったときの処理（必要なら）
    }

}
