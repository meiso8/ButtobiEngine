#include "Model.h"
#include"DirectXCommon.h"

#include"MakeMatrix.h"
#include"Transform.h"
#include<numbers>
#include"ModelManager.h"
#include"MyEngine.h"

Model::~Model()
{
    Finalize();
}

void Model::Create() {

    modelConfig_ = ModelConfig::GetInstance();
    textureHandle_ = modelData_->material.textureSrvIndex;
    CreateVertex();

    CreatePointLightData();
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

void Model::Draw(ID3D12GraphicsCommandList* commandList) {

    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
 
    //SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    SrvManager::SetGraphicsRootDescriptorTable(2, textureHandle_);
 
    //LightのCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(3, modelConfig_->directionalLightResource->GetGPUVirtualAddress());

    //pointLightのCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(7, pointLightResource_->GetGPUVirtualAddress());
    //描画!(DrawCall/ドローコール)。3頂点で1つのインスタンス。インスタンスについては今後
    commandList->DrawInstanced(UINT(modelData_->vertices.size()), 1, 0, 0);

}
