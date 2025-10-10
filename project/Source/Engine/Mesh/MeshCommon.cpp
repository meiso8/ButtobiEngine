#include "MeshCommon.h"
#include"DirectXCommon.h"
#include"MyEngine.h"

ModelConfig* MeshCommon::modelConfig_ = nullptr;
ID3D12GraphicsCommandList* MeshCommon::commandList_ = nullptr;

void MeshCommon::Draw(Camera& camera, const Matrix4x4& worldMatrix, const uint32_t lightType
) {

};

void MeshCommon::CreateVertex()
{
 
}

void MeshCommon::CreateIndexResource() {

}

void MeshCommon::PreDraw(const BlendMode& blendMode) {
    commandList_->SetGraphicsRootSignature(modelConfig_->rootSignature->GetRootSignature(0));
    commandList_->SetPipelineState(MyEngine::GetPSO(blendMode)->GetGraphicsPipelineState(PSO::TRIANGLE).Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


void MeshCommon::SetColor(const Vector4& color) {
    materialResource_.SetColor(color);
}

void MeshCommon::CreateTransformationMatrix() {

    //Matrix4x4　1つ分のサイズを用意
    transformationMatrixResource_ = DirectXCommon::CreateBufferResource(sizeof(TransformationMatrix));
    //データを書き込む
    //書き込むためのアドレスを取得
    transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));
}

void MeshCommon::CreateMaterial(const Vector4& color, uint32_t lightType) {
    //マテリアルリソースを作成
    materialResource_.CreateMaterial(color, lightType);
}

void MeshCommon::CreateWaveData()
{
    int waveCount = 2;
    //waveResource_ = DirectXCommon::CreateBufferResource(sizeof(Wave) * waveCount);
    size_t bufferSize = (sizeof(Wave) * waveCount + 255) & ~255;
    waveResource_ = DirectXCommon::CreateBufferResource(bufferSize);
    //書き込むためのアドレスを取得
    waveResource_->Map(0, nullptr, reinterpret_cast<void**>(&waveData_));

    waveData_[0].direction = { 1.0f,0.0f,0.0f };
    waveData_[0].time = 0.0f;
    waveData_[0].amplitude = 0.0f;
    waveData_[0].frequency = 4;

    waveData_[1].direction = { 1.0f,0.0f,0.0f };
    waveData_[1].time = 0.0f;
    waveData_[1].amplitude = 0.0f;
    waveData_[1].frequency = 4;
}

void MeshCommon::CreateBalloonData()
{
    expansionResource_ = DirectXCommon::CreateBufferResource(sizeof(Balloon));

    //書き込むためのアドレスを取得
    expansionResource_->Map(0, nullptr, reinterpret_cast<void**>(&balloonData_));
    //データを書き込む
    balloonData_->expansion = 0.0f;
    balloonData_->sphere = 0.0f;
    balloonData_->cube = 0.0f;
    balloonData_->isSphere = false;

}
