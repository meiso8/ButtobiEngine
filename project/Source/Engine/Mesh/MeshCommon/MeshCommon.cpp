#include "MeshCommon.h"
#include"DirectXCommon.h"
#include"Texture.h"
#include"PSO.h"

void MeshCommon::Finalize() {

    vertexResource_.Reset();
    indexResource_.Reset();
}

void MeshCommon::PreDraw(ID3D12GraphicsCommandList* commandList, const BlendMode& blendMode, const CullMode& cullMode) {
    commandList->SetGraphicsRootSignature(PSO::GetRootSignature()->GetRootSignature(RootSignature::NORMAL));
    commandList->SetPipelineState(PSO::GetGraphicsPipelineState(blendMode, cullMode).Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


void MeshCommon::SetTextureHandle(const Texture::TEXTURE_HANDLE& textureHandle)
{
    textureHandle_ = Texture::GetHandle(textureHandle);
}

void MeshCommon::CreateIndexResource()
{
}
