#include "LineMesh.h"

#include"DirectXCommon.h"
#include"PSO.h"
#include"SRVmanager/SrvManager.h"

LineMesh::~LineMesh() {
    Finalize();
}

void LineMesh::Create(const Texture::TEXTURE_HANDLE& textureHandle)
{


    textureHandle_ = Texture::GetHandle(textureHandle);

    CreateVertex();
    //CreateIndexResource();

}

void LineMesh::CreateVertex() {

    //VertexResourceとVertexBufferViewを用意
    vertexResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * 2);

    //頂点バッファビューを作成する
    //リソースの先頭アドレスから使う
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    //使用するリソースのサイズ頂点2つ分のサイズ
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * 2;
    //1頂点あたりのサイズ
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

#pragma region //頂点データの設定

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));


    SetVertexData({ -0.5f,0.0f,0.0f }, { 0.5f, 0.0f,0.0f });

    vertexResource_->Unmap(0, nullptr);
#pragma endregion

}

void LineMesh::SetVertexData(const Vector3& start, const Vector3& end) {
    vertexData_[0].position = { start.x,start.y,start.z,1.0f };//左下
    vertexData_[0].texcoord = { 0.0f,1.0f };
    vertexData_[0].normal = { vertexData_[0].position.x,  vertexData_[0].position.y,  vertexData_[0].position.z };//法線
    vertexData_[1].position = { end.x,end.y,end.z,1.0f };//右下
    vertexData_[1].texcoord = { 0.0f,0.0f };
    vertexData_[1].normal = { vertexData_[1].position.x,  vertexData_[1].position.y,  vertexData_[1].position.z };
};

void LineMesh::PreDraw(ID3D12GraphicsCommandList* commandList,  const BlendMode& blendMode,const CullMode& cullMode) {

    commandList->SetGraphicsRootSignature(PSO::rootSignature->GetRootSignature(RootSignature::NORMAL));
    commandList->SetPipelineState(PSO::GetGraphicsPipelineStateLine().Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

    (void)blendMode;
    (void)cullMode;

}

void LineMesh::Draw(ID3D12GraphicsCommandList* commandList)
{

    //頂点バッファビューを設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
   
//SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    SrvManager::SetGraphicsRootDescriptorTable(2, textureHandle_);  
    
    //描画!（DrawCall/ドローコール）
    commandList->DrawInstanced(2, 1, 0, 0);
};

