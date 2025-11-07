#include "Particle.h"
#include"DirectXCommon.h"
#include"Camera/Camera.h"
#include"MakeMatrix.h"
#include"TextureManager.h"
#include"MyEngine.h"
#include"Random.h"
#include"Collision.h"

using namespace  Microsoft::WRL;

ID3D12GraphicsCommandList* ParticleManager::commandList_ = nullptr;

std::list<Particle> Emit(const bool& isRandom, const Emitter& emitter, const Vector3& scale, const Vector4& color)
{
    std::list<Particle>particles;
    for (uint32_t count = 0; count < emitter.cont; ++count) {
        particles.push_back(MakeNewParticle(isRandom, emitter.transform.translate, scale, color));
    }
    return particles;

}

Particle MakeNewParticle(const bool& isRandom, const Vector3& translate, const Vector3& scale, const Vector4& color)
{
    Random::SetMinMax(-1.0f, 1.0f);
    Particle particle;
    particle.transform.scale = scale;
    particle.transform.rotate = { 0.0f,0.0f,0.0f };
    if (isRandom) {
        Vector3 randomTranslate{ Random::Get(), Random::Get(), Random::Get() };
        particle.transform.translate = randomTranslate + translate;
    } else {
        particle.transform.translate = translate;
    }

    particle.velocity = { Random::Get(), Random::Get(), Random::Get() };

    particle.color = color;

    particle.lifeTime = Random::Get();
    particle.currentTime = 0;

    return particle;
}



ParticleManager::~ParticleManager()
{
    Finalize();
}

void ParticleManager::Create(uint32_t textureHandle, int modelHandle)
{

    rootSignature_ = PSO::GetRootSignature();
    commandList_ = DirectXCommon::GetCommandList();

    //モデルデータの作成後にInstancingを作成
    CreateModelData(textureHandle, modelHandle);
    //Instancingを作成
    CreateTransformationMatrix();
    //マテリアルリソースを作成 //ライトなし
    materialResource_ = std::make_unique<MaterialResource>();
    materialResource_->CreateMaterial({ 1.0f,1.0f,1.0f,1.0f }, LightMode::klightModeNone);

    CreateVertexBufferResource();

    InitEmitter();
    InitAccelerationField();

}



void ParticleManager::TimerUpdate(const bool& isRandom, const Vector3& scale, const Vector4& color)
{
    emitter_.frequencyTime += kDeltaTime;

    if (emitter_.frequency <= emitter_.frequencyTime) {
        emitter_.frequencyTime -= emitter_.frequency;
        EmitParticle(isRandom, scale, color);
    }
}

void ParticleManager::Update(Camera& camera)
{

    UpdateBillBordMatrix(camera);

    numInstance_ = 0;

    for (std::list <Particle>::iterator particleIterator = particles.begin(); particleIterator != particles.end();) {

        if (numInstance_ < kNumMaxInstance) {
            if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
                particleIterator = particles.erase(particleIterator);
                continue;
            }

            IsCollisionFieldArea(*particleIterator);

            (*particleIterator).transform.translate += (*particleIterator).velocity * kDeltaTime;

            (*particleIterator).currentTime += kDeltaTime;

            if (useBillboard_) {
                UpdateWorldMatrixForBillBord(*particleIterator);
            } else {
                UpdateWorldMatrix(*particleIterator);
            }

            worldViewProjectionMatrix = Multiply(worldMatrix, camera.GetViewProjectionMatrix());

            instancingData[numInstance_].WVP = worldViewProjectionMatrix;
            instancingData[numInstance_].World = worldMatrix;
            instancingData[numInstance_].color = (*particleIterator).color;
            float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
            instancingData[numInstance_].color.w = alpha;

            ++numInstance_;
        }
        ++particleIterator;
    }


}



void ParticleManager::EmitParticle(const bool& isRandom, const Vector3& scale, const Vector4& color)
{
    particles.splice(particles.end(), Emit(isRandom, emitter_, scale, color));
}

void ParticleManager::Draw(uint32_t blendMode)
{
    if (numInstance_ > 0) {

        //rootSignatureの設定
        commandList_->SetGraphicsRootSignature(rootSignature_->GetRootSignature(RootSignature::PARTICLE));
        commandList_->SetPipelineState(MyEngine::GetPSO()->GetGraphicsPipelineStateParticle(blendMode).Get());
        //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
        commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
        //マテリアルの設定
        commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetMaterialResource()->GetGPUVirtualAddress());
        //粒ごとのトランスフォーム
        commandList_->SetGraphicsRootDescriptorTable(1, instancingSrvHandleGPU);
        //テスクチャ
        commandList_->SetGraphicsRootDescriptorTable(2, TextureManager::GetSrvHandleGPU(textureHandle_));
        //描画!（DrawCall/ドローコール）6個のインデックスを使用しインスタンスを描画。
        commandList_->DrawInstanced(UINT(modelData_->vertices.size()), numInstance_, 0, 0);
    }
}

void ParticleManager::Finalize()
{

    if (materialResource_ != nullptr) {
        materialResource_->UnMap();
        materialResource_ = nullptr;
    }

    if (instancingResource != nullptr) {
        instancingResource->Unmap(0, nullptr);
        instancingResource = nullptr;
    }
}

void ParticleManager::InitEmitter()
{
    emitter_.cont = 3;
    emitter_.frequency = 0.5f;
    emitter_.frequencyTime = 0.0f;
    emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
    emitter_.transform.scale = { 1.0f,1.0f,1.0f };
    emitter_.transform.translate = { 0.0f,0.0f,0.0f };
}

void ParticleManager::InitAccelerationField()
{
    accelerationField.acceleration = { 0.0f,0.0f,0.0f };
    accelerationField.area.min = { -1.0f,-1.0f,-1.0f };
    accelerationField.area.max = { 1.0f,1.0f,1.0f };
}

// ==========================================================================================================-

void ParticleManager::CreateVertexBufferResource()
{
    vertexBufferResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * modelData_->vertices.size());
    vertexBufferResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexBufferData_));
    std::copy(modelData_->vertices.begin(), modelData_->vertices.end(), vertexBufferData_);

    vertexBufferView_.BufferLocation = vertexBufferResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = UINT(sizeof(VertexData) * modelData_->vertices.size());
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

    vertexBufferResource_->Unmap(0, nullptr);

}

void ParticleManager::CreateTransformationMatrix()
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

    D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
    instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
    instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
    instancingSrvDesc.Buffer.FirstElement = 0;
    instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
    instancingSrvDesc.Buffer.NumElements = kNumMaxInstance;
    instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

    //一旦応急処置でtextureHandleに入れる textureのサイス+2分が入る
    uint32_t srvIndex = Texture::AddHandleForSRV(textureHandle_) + 3;
    instancingSrvHandleCPU = DirectXCommon::GetSRVCPUDescriptorHandle(srvIndex);//この書き方はダメですね
    instancingSrvHandleGPU = DirectXCommon::GetSRVGPUDescriptorHandle(srvIndex);
    DirectXCommon::GetDevice()->CreateShaderResourceView(instancingResource.Get(), &instancingSrvDesc, instancingSrvHandleCPU);
}

void ParticleManager::UpdateBillBordMatrix(Camera& camera)
{
    if (useBillboard_) {
        backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
        billboardMatrix = Multiply(backToFrontMatrix, camera.worldMat_);
        billboardMatrix.m[3][0] = 0.0f;
        billboardMatrix.m[3][1] = 0.0f;
        billboardMatrix.m[3][2] = 0.0f;
    }
}

void ParticleManager::UpdateWorldMatrixForBillBord(Particle& particleItr)
{
    Matrix4x4 scaleMatrix = MakeScaleMatrix(particleItr.transform.scale);
    Matrix4x4 translateMatrix = MakeTranslateMatrix(particleItr.transform.translate);
    Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(particleItr.transform.rotate) * billboardMatrix;
    worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;
}

void ParticleManager::UpdateWorldMatrix(Particle& particleItr)
{
    worldMatrix = MakeAffineMatrix(particleItr.transform.scale, particleItr.transform.rotate, particleItr.transform.translate);
}

void ParticleManager::IsCollisionFieldArea(Particle& particleItr)
{
    if (IsCollision(accelerationField.area, particleItr.transform.translate)) {
        particleItr.velocity += accelerationField.acceleration * kDeltaTime;
    }
}

void ParticleManager::CreateModelData(const uint32_t& textureHandle, const int& modelHandle)
{
    if (modelHandle == -1) {
        modelData_ = std::make_unique<ModelData>();
        modelData_->vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
        modelData_->vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f}, .normal = {0.0f,0.0f,1.0f} });//右上
        modelData_->vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//左下
        modelData_->vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//左下
        modelData_->vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f}, .normal = {0.0f,0.0f,1.0f} });//右上
        modelData_->vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f}, .texcoord = {1.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//右下
        modelData_->material.textureFilePath = "./resources/uvChecker.png";
        textureHandle_ = textureHandle;
    } else {
        *modelData_ = ModelManager::GetModelData(modelHandle);
        textureHandle_ = modelData_->textureHandle;
    }

}

