#include "Particle.h"
#include"DirectXCommon.h"
#include"Camera/Camera.h"
#include"MakeMatrix.h"
#include"TextureManager.h"
#include"MyEngine.h"
#include"Random.h"
#include<numbers>
using namespace  Microsoft::WRL;

void ParticleMesh::Initialize(uint32_t textureHandle)
{
    rootSignature_ = PSO::GetRootSignature();
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

    vertexBufferResource_->Unmap(0, nullptr);

    emitter_.cont = 3;
    emitter_.frequency = 0.5f;
    emitter_.frequencyTime = 0.0f;
    emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
    emitter_.transform.scale = { 1.0f,1.0f,1.0f };
    emitter_.transform.translate = { 1.0f,1.0f,1.0f };

}

Particle ParticleMesh::MakeNewParticle(const Vector3& translate)
{
    Random::SetMinMax(-1.0f, 1.0f);
    Particle particle;
    particle.transform.scale = { 1.0f,1.0f,1.0f };
    particle.transform.rotate = { 0.0f,0.0f,0.0f };
    Vector3 randomTranslate{ Random::Get(), Random::Get(), Random::Get() };
    particle.transform.translate = randomTranslate + translate;
    particle.velocity = { Random::Get(), Random::Get(), Random::Get() };

    Random::SetMinMax(0.0f, 1.0f);
    particle.color = { Random::Get(), Random::Get(), Random::Get() ,1.0f };
    Random::SetMinMax(1.0f, 3.0f);
    particle.lifeTime = Random::Get();
    particle.currentTime = 0;


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
    //Instancing用のTransformationMatrixリソースを作成
    instancingResource = DirectXCommon::CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
    //書き込むためのアドレスを取得
    instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));

    assert(instancingResource != nullptr);

    for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
        instancingData[index].WVP = MakeIdentity4x4();
        instancingData[index].World = MakeIdentity4x4();
        instancingData[index].color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
    }
    instancingResource->Unmap(0, nullptr);


    D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
    instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
    instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    instancingSrvDesc.Buffer.FirstElement = 0;
    instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
    instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

    instancingSrvHandleCPU = DirectXCommon::GetSRVCPUDescriptorHandle((UINT)Texture::GetVectorHandles().size() + 1);//この書き方はダメですね
    instancingSrvHandleGPU = DirectXCommon::GetSRVGPUDescriptorHandle((UINT)Texture::GetVectorHandles().size() + 1);
    DirectXCommon::GetDevice()->CreateShaderResourceView(instancingResource.Get(), &instancingSrvDesc, instancingSrvHandleCPU);
}


void ParticleMesh::Draw(Camera& camera, uint32_t blendMode)
{
    if (useBillboard_) {
        backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

        billboardMatrix = Multiply(backToFrontMatrix, camera.worldMat_);
        billboardMatrix.m[3][0] = 0.0f;
        billboardMatrix.m[3][1] = 0.0f;
        billboardMatrix.m[3][2] = 0.0f;
    }

    uint32_t numInstance = 0;

    for (std::list <Particle>::iterator particleIterator = particles.begin(); particleIterator != particles.end();) {

        if (numInstance < kNumMaxInstance) {
            if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
                particleIterator = particles.erase(particleIterator);
                continue;
            }

            (*particleIterator).transform.translate += (*particleIterator).velocity * kDeltaTime;
            (*particleIterator).currentTime += kDeltaTime;

            if (useBillboard_) {
                Matrix4x4 scaleMatrix = MakeScaleMatrix((*particleIterator).transform.scale);
                Matrix4x4 translateMatrix = MakeTranslateMatrix((*particleIterator).transform.translate);
                Matrix4x4 rotateMatrix = MakeRotateXYZMatrix((*particleIterator).transform.rotate) * billboardMatrix;
                worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;
            } else {
                worldMatrix = MakeAffineMatrix((*particleIterator).transform.scale, (*particleIterator).transform.rotate, (*particleIterator).transform.translate);
            }

            Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, camera.GetViewProjectionMatrix());
      /*      instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));*/
            instancingData[numInstance].WVP = worldViewProjectionMatrix;
            instancingData[numInstance].World = worldMatrix;
            instancingData[numInstance].color = (*particleIterator).color;
            float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
            instancingData[numInstance].color.w = alpha;
       /*     instancingResource->Unmap(0, nullptr);*/

            ++numInstance;
        }
        ++particleIterator;
    }


    if (numInstance > 0) {

        ID3D12GraphicsCommandList* commandList = DirectXCommon::GetCommandList();
        PSO* pso = MyEngine::GetPSO();

        //rootSignatureの設定
        commandList->SetGraphicsRootSignature(rootSignature_->GetRootSignature(1));
        commandList->SetPipelineState(pso->GetGraphicsPipelineStateParticle(blendMode).Get());
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
        commandList->DrawInstanced(UINT(modelData_.vertices.size()), numInstance, 0, 0);
    }
}

void ParticleMesh::Update()
{
    emitter_.frequencyTime += kDeltaTime;

    if (emitter_.frequency <= emitter_.frequencyTime) {
        particles.splice(particles.end(), Emit(emitter_));
        emitter_.frequencyTime -= emitter_.frequency;
    }
}

std::list<Particle> Emit(const Emitter& emitter)
{
    std::list<Particle>particles;
    for (uint32_t count = 0; count < emitter.cont; ++count) {
        particles.push_back(ParticleMesh::MakeNewParticle(emitter.transform.translate));
    }
    return particles;

}
