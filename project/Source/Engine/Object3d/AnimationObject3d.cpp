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

#ifdef _DEBUG
    debugBone_ = std::make_unique<DebugBone>();
#endif

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
    animationTime_ = std::fmod(animationTime_, animation_.duration);

    if (modelData_ != nullptr) {

        ApplyAnimation(skeleton_, animation_, animationTime_);
        UpdateSkeleton(skeleton_);

        //Matrix4x4 localMatrix = skeleton_.joints[skeleton_.root].localMatrix;

        if (isSkinning_) {
            worldMatrix_ =  worldTransform_.matWorld_;
        } else {
            worldMatrix_ = modelData_->rootNode.localMatrix * worldTransform_.matWorld_;
        }

    }

#ifdef _DEBUG
    debugBone_->Update(worldTransform_.matWorld_);
#endif
}




void AnimationObject3d::SetMeshAndData(Model* model)
{
    modelData_ = model->GetModelData();
    meshCommon_ = model;
    //試しにここでセットしてみる　
    animation_ = LoadAnimationFileForFilePath(modelData_->filePath);
    skeleton_ = CreateSkeleton(modelData_->rootNode);

#ifdef _DEBUG

    debugBone_->Create(skeleton_);

#endif
}

void AnimationObject3d::Draw(Camera& camera, const BlendMode& blendMode, const CullMode& cullMode)
{
    transformationMatrixData_->World = worldMatrix_;
    transformationMatrixData_->WorldInverseTranspose = Transpose(Inverse(worldMatrix_));
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

#ifdef _DEBUG
    debugBone_->Draw(camera);
#endif
}

