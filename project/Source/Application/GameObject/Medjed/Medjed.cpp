#include "Medjed.h"
#include"ModelManager.h"
#include"MakeMatrix.h"
#include"SkinningModel.h"
#include"CollisionConfig.h"
#include"Sound.h"
#include"TransformAni/TransformAni.h"
#include"Window.h"
#include<algorithm>
#include"Easing.h"
void Medjed::OnCollision(Collider* collider)
{

    if (collider->GetCollisionAttribute() == kCollisionPlayer) {

        if (!isHit_) {
            isHit_ = true;
            if (!isFind_) {
                Sound::PlayOriginSE(Sound::VOICE_Asobimasyo, 0.5f);
            }
        }

    }

    OnCollisionCollider();
}

Vector3 Medjed::GetWorldPosition() const
{
    return aniObj_->worldTransform_.GetWorldPosition();
}

void Medjed::GoToTarget(const Vector3& target)
{
    aniObj_->worldTransform_.translate_ = Lerp(aniObj_->worldTransform_.translate_, target, 0.01f);
}

void Medjed::Hide()
{

    hideTimer_ += InverseFPS;
    hideTimer_ = std::clamp(hideTimer_, 0.0f, 1.0f);



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

void Medjed::Look(const Vector3& target)
{
    Vector3 direction = target - GetWorldPosition();
    aniObj_->worldTransform_.rotate_.y = std::atan2(direction.x, direction.z); // Y軸回転（ラジアン）
}

void Medjed::Update()
{

    if (IsHide()) {
        return;
    }

    isHit_ = false;
    if (isFind_) {
        aniTimer_ += InverseFPS * 0.25f;
        aniObj_->SetColor({ 1.0f,1.0f,1.0f,Easing::EaseInOut(0.0f,1.0f,fmod(aniTimer_,1.0f)) });
    } else {
        SetColor({ 1.0f,1.0f,1.0f,0.0f });
    }

    aniObj_->UpdateAniTimer();
    aniObj_->Update();
    ColliderUpdate();
}
void Medjed::Init()
{
    hideTimer_ = 0.0f;
    aniTimer_ = 0.0f;
    isHit_ = false;
    isFind_ = false;
    aniObj_->Initialize();
    aniObj_->SetAnimation(model_);
    aniObj_->SetColor({ 1.0f,1.0f,1.0f,0.0f });
}

void Medjed::Draw(Camera& camera)
{
    if (IsHide()) {
        return;
    }
    /*   ColliderDraw(camera);*/
    aniObj_->Draw(camera);


}
