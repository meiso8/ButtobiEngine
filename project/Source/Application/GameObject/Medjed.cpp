#include "Medjed.h"
#include"ModelManager.h"
//#include"Model.h"
#include"MakeMatrix.h"
#include"SkinningModel.h"
#include"Input.h"
#include"CollisionConfig.h"

void Medjed::OnCollision(Collider* collider)
{
    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        if (isEnemyApperPtr_) {
            *isEnemyApperPtr_ = true;
        }
    }

    OnCollisionCollider();
}

Vector3 Medjed::GetWorldPosition() const
{
    return object3d_->worldTransform_.GetWorldPosition();
}

Medjed::Medjed() {

    model_ = ModelManager::GetModel(ModelManager::MEDJED);
    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetMesh(model_);


    aniObj_ = std::make_unique<AnimationObject3d>();
    aniObj_->Create();

    skinningModel = std::make_unique<SkinningModel>();
    skinningModel->CreateDatas(ModelManager::GetModel(ModelManager::human_GLTF), ModelManager::GetModel(ModelManager::Ani_GLTF));
    aniObj_->SetMeshAndData(skinningModel.get());

    SetType(kAABB);
    SetAABB(localAABB_);
    SetCollisionAttribute(kCollisionMedjed);
    SetCollisionMask(kCollisionPlayer);

}


void Medjed::LookTarget()
{
    Vector3 direction = *targetPos_ - GetWorldPosition();
    object3d_->worldTransform_.rotate_.y = std::atan2(direction.x, direction.z); // Y軸回転（ラジアン）
    aniObj_->worldTransform_.rotate_.y = object3d_->worldTransform_.rotate_.y;
}


void Medjed::Update()
{
    LookTarget();
    object3d_->Update();

    aniObj_->Update();
    ColliderUpdate();

    if (Input::IsTriggerKey(DIK_SPACE)) {
        aniObj_->InitTime();
        aniObj_->SetAnimation(ModelManager::GetModel(ModelManager::Ani_GLTF));
    }
    if (Input::IsTriggerKey(DIK_RETURN)) {
        aniObj_->InitTime();
        aniObj_->SetAnimation(ModelManager::GetModel(ModelManager::human_GLTF));
    }


}
void Medjed::Init()
{
    object3d_->Initialize();
    aniObj_->Initialize();
}

void Medjed::Draw(Camera& camera)
{
    //object3d_->SetLightMode(kLightModeHalfL);
    //object3d_->Draw(camera);
    ColliderDraw(camera);
    aniObj_->Draw(camera);
   

}
