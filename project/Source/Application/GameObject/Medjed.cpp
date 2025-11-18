#include "Medjed.h"
#include"ModelManager.h"
#include"Model.h"
#include"MakeMatrix.h"
Medjed::Medjed() {

    model_ = ModelManager::GetModel(ModelManager::MEDJED);
    object3d_ = std::make_unique<Object3d>();
    object3d_->Create();
    object3d_->SetMesh(model_);
}

void Medjed::Update()
{
    Matrix4x4 billboardMat =  *targetMatrix_;
    billboardMat.m[3][0] = 0.0f;
    billboardMat.m[3][1] = 0.0f;
    billboardMat.m[3][2] = 0.0f;


    Matrix4x4 scaleMatrix = MakeScaleMatrix(object3d_->worldTransform_.scale_);
    Matrix4x4 translateMatrix = MakeTranslateMatrix(object3d_->worldTransform_.translate_);
    Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(object3d_->worldTransform_.rotate_) * billboardMat;
    object3d_->worldTransform_.matWorld_ = scaleMatrix * rotateMatrix * translateMatrix;


}
void Medjed::Init()
{
    object3d_->Initialize();
    object3d_->worldTransform_.translate_.x = -5.0f;
}

void Medjed::SetTargetMatrix(Matrix4x4* target)
{
    targetMatrix_ = target;
}

void Medjed::Draw(Camera& camera)
{

    object3d_->Draw(camera);

}
