#include "Particle.h"
#include"DirectXCommon.h"
#include"Camera.h"
#include"MakeMatrix.h"
#include"Texture.h"
#include"MyEngine.h"
#include"Random.h"
#include"Collision.h"
#include"SRVmanager/SrvManager.h"
#include"Model.h"

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

    Particle particle;
    particle.transform.scale = scale;
    particle.transform.rotate = { 0.0f,0.0f,0.0f };

    Random::SetMinMax(-1.0f, 1.0f);

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

void ParticleManager::CreateParticleGroup(const std::string name, const uint32_t& textureHandle)
{

    assert(!particleGroups.contains(name));
    std::unique_ptr<ParticleGroup> newParticleGroup = std::make_unique<ParticleGroup>();

    newParticleGroup->materialData.textureFilePath = "./resources/uvChecker.png";
    newParticleGroup->materialData.textureSrvIndex = textureHandle;

    //Instancing用のTransformationMatrixリソースを作成
    newParticleGroup->instancingResource = DirectXCommon::CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
    //書き込むためのアドレスを取得
    newParticleGroup->instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&newParticleGroup->instancingData));

    assert(newParticleGroup->instancingResource != nullptr);

    for (uint32_t index = 0; index < kNumMaxInstance; ++index) {
        newParticleGroup->instancingData[index].WVP = MakeIdentity4x4();
        newParticleGroup->instancingData[index].World = MakeIdentity4x4();
        newParticleGroup->instancingData[index].color = Vector4{ 1.0f,1.0f,1.0f,1.0f };
    }

    //一旦応急処置でtextureHandleに入れる textureのサイス+2分が入る
    newParticleGroup->instanceSrvIndex = SrvManager::Allocate();
    instancingSrvHandleCPU = SrvManager::GetCPUDescriptorHandle(newParticleGroup->instanceSrvIndex);//この書き方はダメですね
    instancingSrvHandleGPU = SrvManager::GetGPUDescriptorHandle(newParticleGroup->instanceSrvIndex);

    SrvManager::CreateSRVforStructuredBuffer(newParticleGroup->instanceSrvIndex, newParticleGroup->instancingResource.Get(), kNumMaxInstance, sizeof(ParticleForGPU));

    //なまえとパーティクルをセットにする
    particleGroups.insert(std::make_pair(name, std::move(newParticleGroup)));

}

void ParticleManager::Create()
{

    rootSignature_ = PSO::GetRootSignature();
    commandList_ = DirectXCommon::GetCommandList();

    //マテリアルリソースを作成 //ライトなし
    materialResource = std::make_unique<MaterialResource>();
    materialResource->CreateMaterial({ 1.0f,1.0f,1.0f,1.0f }, LightMode::klightModeNone);

    CreateModelData();
    CreateVertexBufferResource();
    InitEmitter();
    InitAccelerationField();

}



void ParticleManager::TimerUpdate(const std::string name, const Vector3& position, uint32_t count, const bool& isRandom, const Vector3& scale, const Vector4& color)
{
    emitter_.frequencyTime += kDeltaTime;

    if (emitter_.frequency <= emitter_.frequencyTime) {
        emitter_.frequencyTime -= emitter_.frequency;
        EmitParticle(name, position, count, scale, color, isRandom);
    }
}

void ParticleManager::Update(Camera& camera)
{

    UpdateBillBordMatrix(camera);

    for (const auto& [name, group] : particleGroups) {

        group->numInstance = 0;

        for (std::list <Particle>::iterator particleIterator = group->particles.begin(); particleIterator != group->particles.end();) {

            if (group->numInstance < kNumMaxInstance) {

                //寿命に達していたらグループから外す
                if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
                    particleIterator = group->particles.erase(particleIterator);
                    continue;
                }
                //場の処理
                IsCollisionFieldArea(*particleIterator);
                //移動処理
                (*particleIterator).transform.translate += (*particleIterator).velocity * kDeltaTime;
                //経過時間を加算
                (*particleIterator).currentTime += kDeltaTime;

                //ビルボード処理
                if (useBillboard_) {
                    UpdateWorldMatrixForBillBord(*particleIterator);
                } else {
                    UpdateWorldMatrix(*particleIterator);
                }

                //ビュープロジェクション行列
                worldViewProjectionMatrix = Multiply(worldMatrix, camera.GetViewProjectionMatrix());

                //データにそれぞれ追加
                group->instancingData[group->numInstance].WVP = worldViewProjectionMatrix;
                group->instancingData[group->numInstance].World = worldMatrix;
                group->instancingData[group->numInstance].color = (*particleIterator).color;
                float alpha = 1.0f - ((*particleIterator).currentTime / (*particleIterator).lifeTime);
                group->instancingData[group->numInstance].color.w = alpha;

                ++group->numInstance;
            }
            ++particleIterator;
        }
    }



}



void ParticleManager::EmitParticle(const std::string name, const Vector3& position, uint32_t count, const Vector3& scale, const Vector4& color, const bool& isRandom)
{
    assert(particleGroups.contains(name));
    emitter_.cont = count;
    emitter_.transform.translate = position;

    particleGroups[name]->particles.splice(particleGroups[name]->particles.end(), Emit(isRandom, emitter_, scale, color));


}

void ParticleManager::Draw(uint32_t blendMode)
{
    for (const auto& [name, group] : particleGroups) {

        if (group->numInstance > 0) {
            //rootSignatureの設定
            commandList_->SetGraphicsRootSignature(rootSignature_->GetRootSignature(RootSignature::PARTICLE));
            commandList_->SetPipelineState(MyEngine::GetPSO()->GetGraphicsPipelineStateParticle(blendMode).Get());
            //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
            commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
            //マテリアルの設定
            commandList_->SetGraphicsRootConstantBufferView(0, materialResource->GetMaterialResource()->GetGPUVirtualAddress());
            //粒ごとのトランスフォーム
            SrvManager::SetGraphicsRootDescriptorTable(1, group->instanceSrvIndex);
            //テスクチャ
            SrvManager::SetGraphicsRootDescriptorTable(2, group->materialData.textureSrvIndex);
            //描画!（DrawCall/ドローコール）6個のインデックスを使用しインスタンスを描画。
            commandList_->DrawInstanced(UINT(modelData_->vertices.size()), group->numInstance, 0, 0);

        }

    }
}

void ParticleManager::Finalize()
{

    if (materialResource != nullptr) {
        materialResource->UnMap();
        materialResource = nullptr;
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

void ParticleManager::CreateModelData()
{
    modelData_ = std::make_unique<ModelData>();
    modelData_->vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
    modelData_->vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f}, .normal = {0.0f,0.0f,1.0f} });//右上
    modelData_->vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//左下
    modelData_->vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//左下
    modelData_->vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f}, .normal = {0.0f,0.0f,1.0f} });//右上
    modelData_->vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f}, .texcoord = {1.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//右下

    modelData_->material.textureFilePath = "./Resource/uvChecker.png";
    modelData_->material.textureSrvIndex = Texture::GetHandle(Texture::UV_CHECKER);
}

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
