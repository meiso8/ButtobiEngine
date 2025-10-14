#include "Particle.h"
#include"DirectXCommon.h"
#include"Camera/Camera.h"
#include"MakeMatrix.h"
#include"TextureManager.h"
#include"MyEngine.h"
#include"Random.h"
using namespace  Microsoft::WRL;

void ParticleMesh::Initialize(uint32_t textureHandle)
{
    rootSignature_ = MyEngine::GetRootSignature();
    textureHandle_ = textureHandle;

    CreateModelData();
    CreateTransformationMatrix();

    //マテリアルリソースを作成 //ライトなし
    materialResource_.CreateMaterial({ 1.0f,1.0f,1.0f,1.0f }, MaterialResource::LIGHTTYPE::NONE);

    vertexBufferResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexBufferData_));
    std::copy(modelData_.vertices.begin(), modelData_.vertices.end(), vertexBufferData_);

    vertexBufferView_.BufferLocation = vertexBufferResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_.vertices.size());
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

}

void ParticleMesh::Create()
{
    for (uint32_t index = 0; index < kNumInstance; ++index) {
        particles[index] = MakeNewParticle();
    }

}

void ParticleMesh::Update()
{
    const float kDeltaTime = 1.0f / 60.0f;


    for (uint32_t index = 0; index < kNumInstance; ++index) {

        particles[index].transform.translate += particles[index].velocity * kDeltaTime;
    }

}

Particle ParticleMesh::MakeNewParticle()
{
    Random::SetMinMax(-1.0f, 1.0f);
    Particle particle;
    particle.transform.scale = { 1.0f,1.0f,1.0f };
    particle.transform.rotate = { 0.0f,0.0f,0.0f };
    particle.transform.translate = { Random::Get(), Random::Get(), Random::Get() };
    particle.velocity = { Random::Get(), Random::Get(), Random::Get() };

    Random::SetMinMax(0.0f, 1.0f);
    particle.color = { Random::Get(), Random::Get(), Random::Get() ,1.0f };

    return particle;
}


void ParticleMesh::CreateModelData()
{
    modelData_.vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
    modelData_.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f}, .normal = {0.0f,0.0f,1.0f} });//右上
    modelData_.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//左下
    modelData_.vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//左下
    modelData_.vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f}, .normal = {0.0f,0.0f,1.0f} });//右上
    modelData_.vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f}, .texcoord = {1.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//右下
    modelData_.material.textureFilePath = "./resources/uvChecker.png";

}

void ParticleMesh::CreateTransformationMatrix()
{

    particles.resize(kNumInstance);

    //Instancing用のTransformationMatrixリソースを作成
    instancingResource = DirectXCommon::CreateBufferResource(sizeof(TransformationMatrix) * kNumInstance);
    //書き込むためのアドレスを取得
    instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));

    assert(instancingResource != nullptr);

    for (uint32_t index = 0; index < kNumInstance; ++index) {
        instancingData[index].WVP = MakeIdentity4x4();
        instancingData[index].World = MakeIdentity4x4();
    }

    D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
    instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
    instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    instancingSrvDesc.Buffer.FirstElement = 0;
    instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    instancingSrvDesc.Buffer.NumElements = kNumInstance;
    instancingSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);
    instancingSrvHandleCPU = DirectXCommon::GetSRVCPUDescriptorHandle((UINT)Texture::handle_.size() + 1);//この書き方はダメですね
    instancingSrvHandleGPU = DirectXCommon::GetSRVGPUDescriptorHandle((UINT)Texture::handle_.size() + 1);
    DirectXCommon::GetDevice()->CreateShaderResourceView(instancingResource.Get(), &instancingSrvDesc, instancingSrvHandleCPU);

    for (uint32_t index = 0; index < kNumInstance; ++index) {
        particles[index].transform.scale = { 1.0f,1.0f,1.0f };
        particles[index].transform.rotate = { 0.0f,0.0f,0.0f };
        particles[index].transform.translate = { index * 0.1f,index * 0.1f,index * 0.1f };
        particles[index].velocity = { 0.0f,1.0f,0.0f };
    }

}


void ParticleMesh::Draw(Camera& camera, BlendMode blendMode)
{

    for (uint32_t index = 0; index < kNumInstance; ++index) {
        Matrix4x4 worldMatrix = MakeAffineMatrix(particles[index].transform.scale, particles[index].transform.rotate, particles[index].transform.translate);
        Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, camera.GetViewProjectionMatrix());
        instancingData[index].WVP = worldViewProjectionMatrix;
    }

    ID3D12GraphicsCommandList* commandList = DirectXCommon::GetCommandList();
    PSO* pso = MyEngine::GetPSO(blendMode);

    //rootSignatureの設定
    commandList->SetGraphicsRootSignature(rootSignature_->GetRootSignature(1));
    commandList->SetPipelineState(pso->GetGraphicsPipelineState(PSO::PARTICLE).Get());
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    //マテリアルの設定
    commandList->SetGraphicsRootConstantBufferView(0, materialResource_.GetMaterialResource()->GetGPUVirtualAddress());
    //粒ごとのトランスフォーム
    commandList->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);
    //テスクチャ
    commandList->SetGraphicsRootDescriptorTable(2, TextureManager::GetSrvHandleGPU(textureHandle_));

    //描画!（DrawCall/ドローコール）6個のインデックスを使用しインスタンスを描画。
    commandList->DrawInstanced(UINT(modelData_.vertices.size()), kNumInstance, 0, 0);

}
