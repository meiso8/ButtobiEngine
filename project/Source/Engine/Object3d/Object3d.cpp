#include "Object3d.h"
#include"DirectXCommon.h"
#include"MakeMatrix.h"

ID3D12GraphicsCommandList* Object3d::commandList_ = nullptr;


void Object3d::Draw(Camera& camera, const BlendMode& blendMode,  const CullMode& cullMode)
{

    *transformationMatrixData_ = { Multiply(worldTransform_.matWorld_, camera.GetViewProjectionMatrix()),worldTransform_.matWorld_,Transpose(Inverse(worldTransform_.matWorld_))};

    if (meshCommon_) {
        meshCommon_->PreDraw(commandList_,  blendMode,cullMode);
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
