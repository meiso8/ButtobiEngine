#include "MeshCommon.h"
#include"DirectXCommon.h"
#include"MyEngine.h"
#include"Texture.h"

ModelConfig* MeshCommon::modelConfig_ = nullptr;

void MeshCommon::Finalize() {

    if (materialResource_) {
        materialResource_->UnMap();
        delete materialResource_;
        //materialResource_ = nullptr;
    }

    if (waveResource_) {
        waveResource_->Unmap(0, nullptr);
    }

    if (expansionResource_) {
        expansionResource_->Unmap(0, nullptr);
    }

    vertexResource_.Reset();
    indexResource_.Reset();
    waveResource_.Reset();
    expansionResource_.Reset();

}

void MeshCommon::PreDraw(ID3D12GraphicsCommandList* commandList, const BlendMode& blendMode, const CullMode& cullMode) {
    commandList->SetGraphicsRootSignature(modelConfig_->rootSignature->GetRootSignature(RootSignature::NORMAL));
    commandList->SetPipelineState(PSO::GetGraphicsPipelineState(blendMode, cullMode).Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

void MeshCommon::SetColor(const Vector4& color) {
    materialResource_->SetColor(color);
}

void MeshCommon::InitWaveData()
{
    waveData_[0].direction = { 1.0f,0.0f,0.0f };
    waveData_[0].time = 0.0f;
    waveData_[0].amplitude = 0.0f;
    waveData_[0].frequency = 4;

    waveData_[1].direction = { 1.0f,0.0f,0.0f };
    waveData_[1].time = 0.0f;
    waveData_[1].amplitude = 0.0f;
    waveData_[1].frequency = 4;

}

void MeshCommon::InitBalloonData()
{
    //データを書き込む
    balloonData_->expansion = 0.0f;
    balloonData_->sphere = 0.0f;
    balloonData_->cube = 0.0f;
    balloonData_->isSphere = false;
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


void MeshCommon::CreateMaterial(const Vector4& color, const uint32_t& lightType) {
    //マテリアルリソースを作成
    materialResource_ = new MaterialResource();
    materialResource_->CreateMaterial(color, lightType);
}

void MeshCommon::CreateWaveData()
{
    int waveCount = 2;
    //waveResource_ = DirectXCommon::CreateBufferResource(sizeof(Wave) * waveCount);
    size_t bufferSize = (sizeof(Wave) * waveCount + 255) & ~255;
    waveResource_ = DirectXCommon::CreateBufferResource(bufferSize);
    //書き込むためのアドレスを取得
    waveResource_->Map(0, nullptr, reinterpret_cast<void**>(&waveData_));

    InitWaveData();
}

void MeshCommon::CreateBalloonData()
{
    expansionResource_ = DirectXCommon::CreateBufferResource(sizeof(Balloon));

    //書き込むためのアドレスを取得
    expansionResource_->Map(0, nullptr, reinterpret_cast<void**>(&balloonData_));
    //データを初期化する
    InitBalloonData();

}

void MeshCommon::CreatePointLightData()
{

    pointLightResource_ = DirectXCommon::CreateBufferResource(sizeof(PointLight));

    //書き込むためのアドレスを取得
    pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));
    //データを初期化する
    InitPointLightData();

}
