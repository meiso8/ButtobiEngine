#include "Particle.h"
#include"DirectXCommon.h"
#include"Camera.h"
#include"MakeMatrix.h"
#include"MyEngine.h"
#include"Random.h"
#include"Collision.h"
#include"SRVmanager/SrvManager.h"
#include"Model.h"
#include"SpriteCamera.h"

using namespace  Microsoft::WRL;

ParticleManager* ParticleManager::instance_ = nullptr;
ID3D12GraphicsCommandList* ParticleManager::commandList_ = nullptr;
std::unordered_map<std::string, std::unique_ptr <ParticleGroup> >ParticleManager::particleGroups;
const float ParticleManager::kDeltaTime = 1.0f / 60.0f;

ParticleManager::ParticleManager()
{


}
void ParticleManager::Create()
{
    rootSignature_ = PSO::GetRootSignature();
    commandList_ = DirectXCommon::GetCommandList();

    UpdateFunctions = {
     {Movements::kNormal, std::bind(&ParticleManager::Normal, this)},
     {Movements::kSphere, std::bind(&ParticleManager::Sphere, this)},
     {Movements::kShock, std::bind(&ParticleManager::Shock, this)},
    };

    InitAccelerationField();

    //マテリアルリソースを作成 //ライトなし
    materialResource = std::make_unique<MaterialResource>();
    materialResource->CreateMaterial({ 1.0f,1.0f,1.0f,1.0f }, LightMode::kLightModeNone);
    CreateModelData();

    CreateVertexBufferResource();
}

Particle MakeNewParticle(const bool& isRandom, const WorldTransform& transform, const Vector4& color, const float& lifeTime)
{
    Particle particle;

    Random::SetMinMax(-1.0f, 1.0f);
    particle.transform.scale = transform.scale_;
    particle.transform.rotate = transform.rotate_;
    particle.transform.translate = (isRandom) ? Vector3{ Random::Get(), Random::Get(), Random::Get() } + transform.GetWorldPosition() : transform.GetWorldPosition();
    particle.velocity = { Random::Get(), Random::Get(), Random::Get() };
    particle.color = color;
    particle.lifeTime = (lifeTime == -1.0f) ? Random::Get() : lifeTime;

    particle.currentTime = 0;

    return particle;
}

SphericalCoordinate MakeNewSphericalCoordinate(const float& radius)
{
    SphericalCoordinate sphericalCoordinate;
    Random::SetMinMax(0.0f, 6.28f);
    sphericalCoordinate.azimuthal = 0.0f;
    sphericalCoordinate.polar = Random::Get();
    sphericalCoordinate.radius = radius;
    return sphericalCoordinate;
}

std::list<SphericalCoordinate> EmitCoordinate(const bool& isRandom, const Vector3& position, uint32_t count, const Vector3& scale, const Vector4& color)
{
    std::list<SphericalCoordinate>sphericalCoordinates;

    for (uint32_t i = 0; i < count; ++i) {
        sphericalCoordinates.push_back(MakeNewSphericalCoordinate());
    }

    return sphericalCoordinates;

}

void ParticleManager::CreateParticleGroup(const std::string name, const Texture::TEXTURE_HANDLE& textureHandle, const bool& useModel = false, const ModelManager::MODEL_HANDLE& modelHandle)
{

    assert(!particleGroups.contains(name));
    std::unique_ptr<ParticleGroup> newParticleGroup = std::make_unique<ParticleGroup>();
    newParticleGroup->useModel = useModel;
    newParticleGroup->textureSize = { 100.0f,100.0f };
    if (newParticleGroup->useModel) {
        newParticleGroup->model = ModelManager::GetModel(modelHandle);
        newParticleGroup->materialData.textureSrvIndex = newParticleGroup->model->GetModelData()->material.textureSrvIndex;
        newParticleGroup->materialData.textureFilePath = newParticleGroup->model->GetModelData()->material.textureFilePath;
    } else {
        newParticleGroup->materialData.textureSrvIndex = Texture::GetHandle(textureHandle);
        newParticleGroup->materialData.textureFilePath = Texture::GetFilePath(textureHandle);
    }

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

    newParticleGroup->instancingResource->Unmap(0, nullptr);
    newParticleGroup->instanceSrvIndex = SrvManager::Allocate();

    SrvManager::CreateSRVforStructuredBuffer(newParticleGroup->instanceSrvIndex, newParticleGroup->instancingResource.Get(), kNumMaxInstance, sizeof(ParticleForGPU));
    //名前とパーティクルをセットにする
    particleGroups.insert(std::make_pair(name, std::move(newParticleGroup)));

}

void ParticleManager::Update(Camera& camera)
{
    camera_ = &camera;

    if (useSpriteCamera_) {
        //ビルボードをしない
        useBillboard_ = false;
    }

    if (useBillboard_) {
        UpdateBillBordMatrix(camera);
    }
    UpdateFunctions[movements_]();
}


std::list<Particle> Emit(const bool& isRandom, const WorldTransform& transform, uint32_t count, const Vector4& color)
{
    std::list<Particle>particles;
    for (uint32_t i = 0; i < count; ++i) {
        particles.push_back(MakeNewParticle(isRandom, transform, color));
    }
    return particles;
}

void ParticleManager::EmitParticle(const std::string name, const WorldTransform& transform, uint32_t count, const Vector4& color, const bool& isRandom)
{
    assert(particleGroups.contains(name));
    particleGroups[name]->particles.splice(particleGroups[name]->particles.end(), Emit(isRandom, transform, count, color));
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

            //マテリアルの設定
            commandList_->SetGraphicsRootConstantBufferView(0, materialResource->GetMaterialResource()->GetGPUVirtualAddress());
            //粒ごとのトランスフォーム
            SrvManager::SetGraphicsRootDescriptorTable(1, group->instanceSrvIndex);
            //テスクチャ
            SrvManager::SetGraphicsRootDescriptorTable(2, group->materialData.textureSrvIndex);
            //描画!（DrawCall/ドローコール）6個のインデックスを使用しインスタンスを描画。

            if (group->model != nullptr && group->useModel) {
                commandList_->IASetVertexBuffers(0, 1, &group->model->GetVBV());
                commandList_->DrawInstanced(UINT(group->model->GetModelData()->vertices.size()), group->numInstance, 0, 0);
            } else {
                commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);
                commandList_->DrawInstanced(UINT(modelData_->vertices.size()), group->numInstance, 0, 0);
            }


        }

    }
}

void ParticleManager::InitAccelerationField()
{
    accelerationField.acceleration = { 0.0f,0.0f,0.0f };
    accelerationField.area.min = { -1.0f,-1.0f,-1.0f };
    accelerationField.area.max = { 1.0f,1.0f,1.0f };
}

void ParticleManager::Finalize()
{
    for (auto& [name, group] : particleGroups) {
        if (group->instancingResource != nullptr) {
            group->instancingResource = nullptr;
        }
    }

    if (instance_ != nullptr) {
        delete instance_;
        instance_ = nullptr;
    }
}

void ParticleManager::IsCollisionFieldArea(Particle& particleItr)
{
    if (IsCollision(accelerationField.area, particleItr.transform.translate)) {
        particleItr.velocity += accelerationField.acceleration * kDeltaTime;
    }
}

// ==========================================================================================================

void ParticleManager::CreateAll()
{
    CreateParticleGroup("uvChecker", Texture::UV_CHECKER);
    CreateParticleGroup("white", Texture::WHITE_1X1, true, ModelManager::PEOPLE);
}



void ParticleManager::SetMovement(Movements& move)
{
    movements_ = move;
}

void ParticleManager::Normal()
{

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

                UpdateMatrix(*particleIterator, *group);

                //ビュープロジェクション行列
                UpdateWVPMatrix(*camera_);

                //データの更新
                UpdateInstancingData(*group, *particleIterator);

            }
            ++particleIterator;
        }

    }

}

void ParticleManager::Sphere()
{
    for (const auto& [name, group] : particleGroups) {

        group->numInstance = 0;
        auto particleIterator = group->particles.begin();
        auto coordIterator = sphericalCoordinates.begin();

        while (particleIterator != group->particles.end() && coordIterator != sphericalCoordinates.end()) {

            if (group->numInstance < kNumMaxInstance) {

                if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
                    particleIterator = group->particles.erase(particleIterator);
                    continue;
                }

                (*particleIterator).currentTime += kDeltaTime;

                coordIterator->polar += std::numbers::pi_v<float> *kDeltaTime * 4.0f;

                if (coordIterator->radius > 0.0f) {
                    coordIterator->radius -= kDeltaTime * 4.0f;
                } else {
                    coordIterator->radius = 5.0f;
                }


                IsCollisionFieldArea(*particleIterator);

                particleIterator->transform.translate += TransformCoordinate(*coordIterator);

                float time = ((*particleIterator).currentTime / (*particleIterator).lifeTime);

                particleIterator->transform.scale.x = time * 0.5f;
                particleIterator->transform.scale.y = time * 0.5f;
                particleIterator->transform.scale.z = time * 0.5f;

                UpdateWorldMatrix(*particleIterator, *group);

                UpdateWVPMatrix(*camera_);

                UpdateInstancingData(*group, *particleIterator);

            }

            ++particleIterator;
            ++coordIterator;
        }
    }

}

void ParticleManager::Shock()
{



}

// ==========================================================================================================

void ParticleManager::UpdateInstancingData(ParticleGroup& group, Particle& particleItr)
{

    group.instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&group.instancingData));

    //データにそれぞれ追加
    group.instancingData[group.numInstance].WVP = worldViewProjectionMatrix;
    group.instancingData[group.numInstance].World = worldMatrix;
    group.instancingData[group.numInstance].color = (particleItr).color;
    float alpha = 1.0f - ((particleItr).currentTime / (particleItr).lifeTime);
    group.instancingData[group.numInstance].color.w = alpha;

    group.instancingResource->Unmap(0, nullptr);

    ++group.numInstance;

}


ParticleManager* ParticleManager::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new ParticleManager();
    }
    return instance_;
}

std::unordered_map<std::string, std::unique_ptr<ParticleGroup>>& ParticleManager::GetParticleGroups()
{
    return particleGroups;
}

// ==========================================================================================================

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

void ParticleManager::UpdateWVPMatrix(Camera& camera)
{

    if (useSpriteCamera_) {
        worldViewProjectionMatrix = Multiply(worldMatrix, SpriteCamera::GetViewProjectionMatrix());
    } else {
        worldViewProjectionMatrix = Multiply(worldMatrix, camera.GetViewProjectionMatrix());
    }

}

void ParticleManager::UpdateMatrix(Particle& particleItr, ParticleGroup& group)
{
    //ビルボード処理
    if (useBillboard_) {
        UpdateWorldMatrixForBillBord(particleItr);
    } else {
        UpdateWorldMatrix(particleItr, group);
    }
}
void ParticleManager::UpdateWorldMatrixForBillBord(Particle& particleItr)
{
    Matrix4x4 scaleMatrix = MakeScaleMatrix(particleItr.transform.scale);
    Matrix4x4 translateMatrix = MakeTranslateMatrix(particleItr.transform.translate);
    Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(particleItr.transform.rotate) * billboardMatrix;
    worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;
}

void ParticleManager::UpdateWorldMatrix(Particle& particleItr, ParticleGroup& group)
{

    if (useSpriteCamera_) {
        worldMatrix = MakeAffineMatrix(particleItr.transform.scale * group.textureSize, particleItr.transform.rotate, particleItr.transform.translate * group.textureSize);
    } else {
        worldMatrix = MakeAffineMatrix(particleItr.transform.scale, particleItr.transform.rotate, particleItr.transform.translate);
    }

}

// ==========================================================================================================

void ParticleManager::CreateModelData()
{
    modelData_ = std::make_unique<ModelData>();
    modelData_->vertices.push_back({ .position = {1.0f,1.0f,0.0f,1.0f},.texcoord = {0.0f,0.0f},.normal = {0.0f,0.0f,1.0f} });//左上
    modelData_->vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f}, .normal = {0.0f,0.0f,1.0f} });//右上
    modelData_->vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//左下
    modelData_->vertices.push_back({ .position = {1.0f,-1.0f,0.0f,1.0f}, .texcoord = {0.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//左下
    modelData_->vertices.push_back({ .position = {-1.0f,1.0f,0.0f,1.0f}, .texcoord = {1.0f,0.0f}, .normal = {0.0f,0.0f,1.0f} });//右上
    modelData_->vertices.push_back({ .position = {-1.0f,-1.0f,0.0f,1.0f}, .texcoord = {1.0f,1.0f}, .normal = {0.0f,0.0f,1.0f} });//右下
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