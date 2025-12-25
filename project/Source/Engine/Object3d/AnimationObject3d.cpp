#include "AnimationObject3d.h"
#include"Window.h"
#include"Model.h"
#include"ModelData.h"
#include"Lights/PointLightManager.h"
#include"Lights/DirectionalLightManager.h"
#include"Lights/SpotLightManager.h"
#include"MakeMatrix.h"
#include"SkinningModel.h"
#include"Bone.h"
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

    assert(boneModelData_);
    assert( skeleton_ );
    assert(skinCluster_);

    //アニメーションの更新を行って、骨ごとのLocal情報を更新する
    ApplyAnimation(*skeleton_, animation_, animationTime_);
    //現在の骨ごとのLocal情報を基にSkeletonSpaceの情報を更新する
    UpdateSkeleton(*skeleton_);
    //SkeletonSpaceの情報を基に、SkinClusterのMatrixPaletteを更新する
    UpdateSkinCluster(*skinCluster_, *skeleton_);

    if (isSkinning_) {
        worldMatrix_ = worldTransform_.matWorld_;
    } else {
        worldMatrix_ = boneModelData_->rootNode.localMatrix * worldTransform_.matWorld_;
    }

#ifdef _DEBUG
    debugBone_->Update(worldTransform_.matWorld_);
#endif
}
void AnimationObject3d::SetMeshAndData(SkinningModel* skinningModel)
{

    skinningModel_ = skinningModel;

    boneModelData_ = skinningModel_->GetBoneModelData();
    skeleton_ = skinningModel_->GetSkeleton();
    skinCluster_ = skinningModel_->GetSkinCluster();

    //試しにここでセットしてみる　
    animation_ = AnimationManager::GetAnimation(boneModelData_->filePath);

#ifdef _DEBUG

    debugBone_->Create(*skeleton_);

#endif
}

void AnimationObject3d::SetTextureHandle(const Texture::TEXTURE_HANDLE& handle)
{
    skinningModel_->SetTextureHandle(handle);
}

void AnimationObject3d::Draw(Camera& camera, const BlendMode& blendMode, const CullMode& cullMode)
{
    transformationMatrixData_->World = worldMatrix_;
    transformationMatrixData_->WorldInverseTranspose = Transpose(Inverse(worldMatrix_));
    transformationMatrixData_->WVP = Multiply(worldMatrix_, camera.GetViewProjectionMatrix());

    if (skinningModel_) {
        skinningModel_->PreDraw(commandList_, blendMode, cullMode);
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
        skinningModel_->Draw(commandList_);
    }

#ifdef _DEBUG
    debugBone_->Draw(camera);
#endif

}

