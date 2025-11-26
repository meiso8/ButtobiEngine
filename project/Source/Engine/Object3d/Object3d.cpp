#include "Object3d.h"
#include"DirectXCommon.h"
#include"MakeMatrix.h"

ID3D12GraphicsCommandList* Object3d::commandList_ = nullptr;


void Object3d::CreateMaterial(const Vector4& color, const uint32_t& lightType) {
    //マテリアルリソースを作成
    materialResource_ = std::make_unique<MaterialResource>();
    materialResource_->CreateMaterial(color, lightType);
}


void Object3d::CreateUV()
{
    uvTransform_ = {
      {1.0f,1.0f,1.0f},
      {0.0f,0.0f,0.0f},
      {0.0f,0.0f,0.0f},
    };

    uvTransformMatrix_ = MakeIdentity4x4();

};

void Object3d::UpdateUV() {

    uvTransformMatrix_ = MakeAffineMatrix(uvTransform_.scale, uvTransform_.rotate, uvTransform_.translate);
    materialResource_->SetUV(uvTransformMatrix_);
}


void Object3d::Draw(Camera& camera, const BlendMode& blendMode, const CullMode& cullMode)
{

    *transformationMatrixData_ = { Multiply(worldTransform_.matWorld_, camera.GetViewProjectionMatrix()),worldTransform_.matWorld_,Transpose(Inverse(worldTransform_.matWorld_)) };

    if (meshCommon_) {
        meshCommon_->PreDraw(commandList_, blendMode, cullMode);
        //マテリアルCBufferの場所を設定　/*RotParameter配列の0番目 0->register(b4)1->register(b0)2->register(b4)*/
        commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetMaterialResource()->GetGPUVirtualAddress());
        //wvp用のCBufferの場所を設定
        commandList_->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
        //cameraのCBufferの場所を設定
        commandList_->SetGraphicsRootConstantBufferView(6, camera.GetResource()->GetGPUVirtualAddress());
        meshCommon_->Draw(commandList_);
    }
}

void Object3d::Create()
{
    commandList_ = DirectXCommon::GetCommandList();
    CreateTransformationMatrix();
    CreateMaterial();
    Initialize();
}

void Object3d::Initialize()
{
    worldTransform_.Initialize();
}

void Object3d::Update()
{
    WorldTransformUpdate(worldTransform_);
}


void Object3d::CreateTransformationMatrix() {

    //Matrix4x4　1つ分のサイズを用意
    transformationMatrixResource_ = DirectXCommon::CreateBufferResource(sizeof(TransformationMatrixFor3D));
    //データを書き込む
    //書き込むためのアドレスを取得
    transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
    transformationMatrixData_->WVP = MakeIdentity4x4();
    transformationMatrixData_->World = MakeIdentity4x4();

    transformationMatrixData_->WorldInverseTranspose = MakeIdentity4x4();

}
