#include "AnimationObject3d.h"
#include"Window.h"
#include"Model.h"
#include"ModelData.h"
#include"Lights/PointLightManager.h"
#include"Lights/DirectionalLightManager.h"
#include"Lights/SpotLightManager.h"
#include"MakeMatrix.h"

AnimationObject3d::AnimationObject3d() {

    animationTime_ = 0.0f;
    rotate_ = IdentityQuaternion();
    worldMatrix_ = MakeIdentity4x4();
}

void AnimationObject3d::Update()
{
    WorldTransformUpdate(worldTransform_);
    UpdateAnimation();
}

void AnimationObject3d::InitTime()
{
    animationTime_ = 0.0f;
}

void AnimationObject3d::UpdateAnimation()
{
    animationTime_ += InverseFPS;
    animationTime_ = std::fmod(animationTime_, animation.duration);

    if (modelData_ != nullptr) {
        NodeAnimation& rootNodeAnimation = animation.nodeAnimations[modelData_->rootNode.name];
        Vector3 translate = CalculateValue(rootNodeAnimation.translate.keyframes, animationTime_);
        Quaternion rotate = CalculateValue(rootNodeAnimation.rotate.keyframes, animationTime_);
        Vector3 scale = CalculateValue(rootNodeAnimation.scale.keyframes, animationTime_);
        Matrix4x4 localMatrix = MakeAffineMatrix(scale, rotate, translate);

        worldMatrix_ = localMatrix * modelData_->rootNode.localMatrix * worldTransform_.matWorld_;
        transformationMatrixData_->World = worldMatrix_;
        transformationMatrixData_->WorldInverseTranspose = Transpose(Inverse(worldMatrix_));
    }
}

void AnimationObject3d::SetMeshAndData(Model* model)
{
    modelData_ = model->GetModelData();
    meshCommon_ = model;
    //試しにここでセットしてみる　
    animation = LoadAnimationFileForFilePath(modelData_->filePath);
}

void AnimationObject3d::Draw(Camera& camera, const BlendMode& blendMode, const CullMode& cullMode)
{

    transformationMatrixData_->WVP = Multiply(worldMatrix_, camera.GetViewProjectionMatrix());

    if (meshCommon_) {
        meshCommon_->PreDraw(commandList_, blendMode, cullMode);
        //マテリアルCBufferの場所を設定　/*RotParameter配列の0番目 0->register(b4)1->register(b0)2->register(b4)*/
        commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetMaterialResource()->GetGPUVirtualAddress());
        //wvp用のCBufferの場所を設定
        commandList_->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
        //timeのSRVの場所を設定
        commandList_->SetGraphicsRootShaderResourceView(4, waveResource_->GetGPUVirtualAddress());
        //expansionのCBufferの場所を設定
        commandList_->SetGraphicsRootConstantBufferView(5, expansionResource_->GetGPUVirtualAddress());
        //cameraのCBufferの場所を設定
        commandList_->SetGraphicsRootConstantBufferView(6, camera.GetResource()->GetGPUVirtualAddress());
        //ライトのCBufferの場所を設定
        DirectionalLightManager::SetGraphicsRootConstantBufferView();
        PointLightManager::SetGraphicsRootDescriptorTable();
        SpotLightManager::SetGraphicsRootConstantBufferView();
        meshCommon_->Draw(commandList_);
    }
}
