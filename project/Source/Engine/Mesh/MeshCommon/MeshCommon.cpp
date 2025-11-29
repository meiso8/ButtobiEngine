#include "MeshCommon.h"
#include"DirectXCommon.h"
#include"MyEngine.h"
#include"Texture.h"

ModelConfig* MeshCommon::modelConfig_ = nullptr;

void MeshCommon::Finalize() {

    vertexResource_.Reset();
    indexResource_.Reset();
  
}

void MeshCommon::PreDraw(ID3D12GraphicsCommandList* commandList, const BlendMode& blendMode, const CullMode& cullMode) {
    commandList->SetGraphicsRootSignature(modelConfig_->rootSignature->GetRootSignature(RootSignature::NORMAL));
    commandList->SetPipelineState(PSO::GetGraphicsPipelineState(blendMode, cullMode).Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void MeshCommon::InitPointLightData()
{
    pointLightData_->color = { 1.0f,1.0f,1.0f,1.0f };
    pointLightData_->intensity = 1.0f;
    pointLightData_->position = { 0.0f,0.0f,0.0f };
}

void MeshCommon::SetTextureHandle(const Texture::TEXTURE_HANDLE& textureHandle)
{
    textureHandle_ = Texture::GetHandle(textureHandle);


}

void MeshCommon::CreateIndexResource()
{
}

void MeshCommon::CreatePointLightData()
{

    pointLightResource_ = DirectXCommon::CreateBufferResource(sizeof(PointLight));

    //書き込むためのアドレスを取得
    pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
    //データを初期化する
    InitPointLightData();

}
