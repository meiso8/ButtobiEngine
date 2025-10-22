#include "Model.h"
#include"DirectXCommon.h"

#include"TransformationMatrix.h"
#include"MakeMatrix.h"
#include"Transform.h"
#include<numbers>
#include"TextureManager.h"
#include"ModelManager.h"
#include"MyEngine.h"

void Model::Create(const ModelManager::MODEL_HANDLE& modelHandle) {

    modelData_ = &ModelManager::GetModelData(modelHandle);
    modelConfig_ = ModelConfig::GetInstance();
    commandList_ = DirectXCommon::GetCommandList();
    textureHandle_ = modelData_->textureHandle;

    //マテリアルの作成 lightType halfLambert
    CreateMaterial({ 1.0f,1.0f,1.0f,1.0f }, MaterialResource::HALF_L);

    CreateTransformationMatrix();
    CreateVertex();
    CreateUV();
    CreateWaveData();
    CreateBalloonData();
}


void Model::CreateVertex() {
    //頂点リソースを作る
    vertexResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * modelData_->vertices.size());

    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();//リソースの先頭アドレスから使う
    vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_->vertices.size());//使用するリソースのサイズは頂点のサイズ
    vertexBufferView_.StrideInBytes = sizeof(VertexData);//1頂点あたりのサイズ

    //頂点リソースにデータを書き込む
    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));//書き込むためのアドレスを取得
    std::memcpy(vertexData_, modelData_->vertices.data(), sizeof(VertexData) * modelData_->vertices.size());//頂点データをリソースにコピー
    vertexResource_->Unmap(0, nullptr);

};


void Model::CreateUV() {
    uvTransform_ = {
    {1.0f,1.0f,1.0f},
    {0.0f,0.0f,0.0f},
    {0.0f,0.0f,0.0f},
    };

    uvTransformMatrix_ = MakeIdentity4x4();
}

void Model::UpdateUV() {

    uvTransformMatrix_ = MakeAffineMatrix(uvTransform_.scale, uvTransform_.rotate, uvTransform_.translate);
    materialResource_.SetUV(uvTransformMatrix_);
}

void Model::PreDraw(const BlendMode& type) {
    commandList_->SetGraphicsRootSignature(modelConfig_->rootSignature->GetRootSignature(RootSignature::NORMAL));
    commandList_->SetPipelineState(MyEngine::GetPSO()->GetGraphicsPipelineState(type,kCullModeBack).Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::Draw(Camera& camera, const Matrix4x4& worldMatrix, const uint32_t lightType) {

    materialResource_.SetLightType(lightType);

    worldViewProjectionMatrix_ = Multiply(worldMatrix, camera.GetViewProjectionMatrix());
    //データを書き込む

    *transformationMatrixData_ = { worldViewProjectionMatrix_,worldMatrix };


    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
    //マテリアルCBufferの場所を設定　/*RotParameter配列の0番目 0->register(b4)1->register(b0)2->register(b4)*/
    commandList_->SetGraphicsRootConstantBufferView(0, materialResource_.GetMaterialResource()->GetGPUVirtualAddress());
    //wvp用のCBufferの場所を設定
    commandList_->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
    //SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    commandList_->SetGraphicsRootDescriptorTable(2, TextureManager::GetSrvHandleGPU(textureHandle_));
    //LightのCBufferの場所を設定
    commandList_->SetGraphicsRootConstantBufferView(3, modelConfig_->directionalLightResource->GetGPUVirtualAddress());
    //timeのSRVの場所を設定
    commandList_->SetGraphicsRootShaderResourceView(4, waveResource_->GetGPUVirtualAddress());
    //expansionのCBufferの場所を設定
    commandList_->SetGraphicsRootConstantBufferView(5, expansionResource_->GetGPUVirtualAddress());
    //描画!(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
    commandList_->DrawInstanced(UINT(modelData_->vertices.size()), 1, 0, 0);

}
