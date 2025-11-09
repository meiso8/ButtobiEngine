#include "Model.h"
#include"DirectXCommon.h"

#include"MakeMatrix.h"
#include"Transform.h"
#include<numbers>
#include"Texture.h"
#include"ModelManager.h"
#include"MyEngine.h"

Model::~Model()
{
    Finalize();
}

void Model::Create() {

    modelConfig_ = ModelConfig::GetInstance();
    textureHandle_ = modelData_->material.textureSrvIndex;

    //マテリアルの作成 lightType halfLambert
    CreateMaterial({ 1.0f,1.0f,1.0f,1.0f }, kLightModeHalfL);
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
    materialResource_->SetUV(uvTransformMatrix_);
}

void Model::Draw(ID3D12GraphicsCommandList* commandList) {

    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
    //マテリアルCBufferの場所を設定　/*RotParameter配列の0番目 0->register(b4)1->register(b0)2->register(b4)*/
    commandList->SetGraphicsRootConstantBufferView(0, materialResource_->GetMaterialResource()->GetGPUVirtualAddress());
    //SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    SrvManager::SetGraphicsRootDescriptorTable(2, textureHandle_);
 
    //LightのCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(3, modelConfig_->directionalLightResource->GetGPUVirtualAddress());
    //timeのSRVの場所を設定
    commandList->SetGraphicsRootShaderResourceView(4, waveResource_->GetGPUVirtualAddress());
    //expansionのCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(5, expansionResource_->GetGPUVirtualAddress());
    //描画!(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
    commandList->DrawInstanced(UINT(modelData_->vertices.size()), 1, 0, 0);

}
