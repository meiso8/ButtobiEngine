#include "Medjed.h"
#include"ModelManager.h"
#include"MakeMatrix.h"
#include"SkinningModel.h"
#include"CollisionConfig.h"

void Medjed::OnCollision(Collider* collider)
{
    //ここはは後でRayと当たったとき且つクリックしたときに変更する
    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
 
    }

    OnCollisionCollider();
}

Vector3 Medjed::GetWorldPosition() const
{
    return aniObj_->worldTransform_.GetWorldPosition();
}

void Medjed::MoveStart()
{
    aniObj_->InitTime();
    aniObj_->SetAnimation(ModelManager::GetModel(ModelManager::medJed_GLTF));
}

Medjed::Medjed() {

    model_ = ModelManager::GetModel(ModelManager::normalMedjed_GLTF);

    aniObj_ = std::make_unique<AnimationObject3d>();
    aniObj_->Create();

    skinningModel = std::make_unique<SkinningModel>();
    skinningModel->CreateDatas(model_, ModelManager::GetModel(ModelManager::normalMedjed_GLTF));
    aniObj_->SetMeshAndData(skinningModel.get());

    SetType(kAABB);
    SetAABB(localAABB_);
    SetCollisionAttribute(kCollisionMedjed);
    SetCollisionMask(kCollisionPlayer);

}

void Medjed::LookTarget(const Vector3& target)
{
    Vector3 direction = target - GetWorldPosition();
    aniObj_->worldTransform_.rotate_.y = std::atan2(direction.x, direction.z); // Y軸回転（ラジアン）
}

void Medjed::Update()
{
    aniObj_->Update();
    ColliderUpdate();
}
void Medjed::Init()
{
    aniObj_->Initialize();
    aniObj_->SetAnimation(model_);
}

void Medjed::Draw(Camera& camera)
{
    /*   ColliderDraw(camera);*/
    aniObj_->Draw(camera);


}
