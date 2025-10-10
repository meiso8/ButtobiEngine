#include "LineMesh.h"

#include"MakeAffineMatrix.h"
#include"Multiply.h"
#include"DirectXCommon.h"
#include"MyEngine.h"
#include"TextureManager.h"

void LineMesh::Create(uint32_t& textureHandle)
{
    commandList_ = DirectXCommon::GetCommandList();
    modelConfig_ = ModelConfig::GetInstance();
    textureHandle_ = textureHandle;

    CreateVertex();
    //CreateIndexResource();

    CreateTransformationMatrix();
    CreateMaterial();
    CreateWaveData();
    CreateBalloonData();

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

void LineMesh::PreDraw(const BlendMode& blendMode) {
    commandList_->SetGraphicsRootSignature(modelConfig_->rootSignature->GetRootSignature(0));
    commandList_->SetPipelineState(MyEngine::GetPSO(blendMode)->GetGraphicsPipelineState(PSO::LINE).Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
}

void LineMesh::Draw(Camera& camera, const Matrix4x4& worldMatrix, const uint32_t lightType)
{
    materialResource_.SetLightType(lightType);

    worldViewProjectionMatrix_ = Multiply(worldMatrix, camera.GetViewProjectionMatrix());
    *transformationMatrixData_ = { worldViewProjectionMatrix_,worldMatrix };

    //頂点バッファビューを設定
    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
    //マテリアルCBufferの場所を設定　/*RotParameter配列の0番目 0->register(b4)1->register(b0)2->register(b4)*/
    commandList_->SetGraphicsRootConstantBufferView(0, materialResource_.GetMaterialResource()->GetGPUVirtualAddress());
    //TransformationMatrixCBufferの場所を設定
    commandList_->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
    //SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    commandList_->SetGraphicsRootDescriptorTable(2, TextureManager::GetSrvHandleGPU(textureHandle_));
    //LightのCBufferの場所を設定
    commandList_->SetGraphicsRootConstantBufferView(3, modelConfig_->directionalLightResource->GetGPUVirtualAddress());
    //timeのSRVの場所を設定
    commandList_->SetGraphicsRootShaderResourceView(4, waveResource_->GetGPUVirtualAddress());
    //expansionのCBufferの場所を設定
    commandList_->SetGraphicsRootConstantBufferView(5, expansionResource_->GetGPUVirtualAddress());

    //描画!（DrawCall/ドローコール）
    commandList_->DrawInstanced(2, 1, 0, 0);
};

