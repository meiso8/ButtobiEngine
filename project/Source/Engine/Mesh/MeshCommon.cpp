#include "MeshCommon.h"
#include"DirectXCommon.h"
#include"MyEngine.h"
#include"Multiply.h"
#include"TextureManager.h"

ModelConfig* MeshCommon::modelConfig_ = nullptr;

void MeshCommon::Create(uint32_t& textureHandle)
{
    commandList_ = DirectXCommon::GetCommandList();
    modelConfig_ = ModelConfig::GetInstance();
    textureHandle_ = textureHandle;

    CreateVertex();
    CreateIndexResource();

    CreateTransformationMatrix();
    CreateMaterial();
    CreateWaveData();
    CreateBalloonData();
}


void MeshCommon::PreDraw(BlendMode blendMode) {
    PSO* pso = MyEngine::GetPSO(blendMode);
    commandList_->SetGraphicsRootSignature(modelConfig_->rootSignature->GetRootSignature(0));
    commandList_->SetPipelineState(pso->GetGraphicsPipelineState(PSO::TRIANGLE).Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void MeshCommon::Draw(Camera& camera, const Matrix4x4& worldMatrix, uint32_t lightType
) {

    materialResource_.SetLightType(lightType);

    worldViewProjectionMatrix_ = Multiply(worldMatrix, camera.GetViewProjectionMatrix());
    *transformationMatrixData_ = { worldViewProjectionMatrix_,worldMatrix };

    //頂点バッファビューを設定
    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
    //IBVを設定new
    commandList_->IASetIndexBuffer(&indexBufferView_);//IBVを設定
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

    //描画!（DrawCall/ドローコール）6個のインデックスを使用し1つのインスタンスを描画。その他は当面0で良い。
    commandList_->DrawIndexedInstanced(36, 1, 0, 0, 0);
};


void MeshCommon::SetColor(const Vector4& color) {
    materialResource_.SetColor(color);
}

void MeshCommon::CreateVertex()
{
 
}

void MeshCommon::CreateIndexResource() {

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
    waveResource_ = DirectXCommon::CreateBufferResource(sizeof(Wave) * waveCount);

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
