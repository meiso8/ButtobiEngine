#include "Particle.h"
#include"DirectXCommon.h"
#include"Camera.h"
#include"MakeMatrix.h"
#include"PSO.h"
#include"Random.h"
#include"Collision.h"
#include"SRVmanager/SrvManager.h"
#include"Model.h"
#include"SpriteCamera.h"
#include"ParticleEmitter.h"
#include"Input.h"

using namespace  Microsoft::WRL;

ParticleManager* ParticleManager::instance_ = nullptr;
ID3D12GraphicsCommandList* ParticleManager::commandList_ = nullptr;
std::unordered_map<std::string, std::unique_ptr <ParticleGroup> >ParticleManager::particleGroups;


// ==========================================================================================================

void ParticleManager::CreateAll()
{
    CreateParticleGroup("enemyHitParticle", Texture::ENEMY_ACTION_PARTICLE/*, true, ModelManager::BOX*/);
    //CreateParticleGroup("enemyWingParticle", Texture::PLAYER_WALK_PARTICLE);
    CreateParticleGroup("playerHitParticle", Texture::PLAYER_DAMAGE_PARTICLE);
    CreateParticleGroup("playerWalkParticle", Texture::PLAYER_WALK_PARTICLE);

    CreateParticleGroup("floorBombBlastParticle", Texture::PLAYER_WALK_PARTICLE);

    CreateParticleGroup("windAttackParticle01", Texture::WIND_ATTACK_PARTICLE01);
    CreateParticleGroup("windAttackParticle02", Texture::WIND_ATTACK_PARTICLE02);
   //葉っぱ
    CreateParticleGroup("leafParticle", Texture::LEAF_PARTICLE);
    //床
    CreateParticleGroup("floorParticle", Texture::FLOOR_PARTICLE);
}

// ==========================================================================================================


ParticleManager::ParticleManager()
{


}
void ParticleManager::Create()
{
    rootSignature_ = PSO::GetRootSignature();
    commandList_ = DirectXCommon::GetCommandList();

    UpdateFunctions = {
        {kParticleNormal, [this](ParticleGroup& group) { Normal(group); }},
        {kParticleSphere, [this](ParticleGroup& group) { Sphere(group); }},
        {kParticleShock, [this](ParticleGroup& group) { Shock(group); }},
    };

    //マテリアルリソースを作成 //ライトなし
    materialResource = std::make_unique<MaterialResource>();
    materialResource->CreateMaterial({ 1.0f,1.0f,1.0f,1.0f }, LightMode::kLightModeNone);
    CreateModelData();

    CreateVertexBufferResource();
}

Particle MakeNewParticle(const MinMax& velocityMinMax, const WorldTransform& transform, const Vector4& color, const float& lifeTime, const AABB& translateAABB, const float& rotateOffset, const float& scaleOffset)
{

    Particle particle;
    Random::SetMinMax(0.0f, 1.0f);
    particle.lifeTime = (lifeTime == -1.0f) ? Random::Get() : lifeTime;
    Random::SetMinMax(velocityMinMax.min, velocityMinMax.max);
    particle.velocity = { Random::Get(), Random::Get(), Random::Get() };

    Random::SetMinMax(-scaleOffset, scaleOffset);
    particle.transform.scale = transform.scale_ + Vector3{ Random::Get(), Random::Get(), Random::Get() };
    
    Vector3 newTransform = transform.GetWorldPosition();
    Random::SetMinMax(translateAABB.min.x, translateAABB.max.x);
    particle.transform.translate.x =  Random::Get()+ newTransform.x;
    Random::SetMinMax(translateAABB.min.y, translateAABB.max.y);
    particle.transform.translate.y = Random::Get() + newTransform.y;
    Random::SetMinMax(translateAABB.min.z, translateAABB.max.z);
    particle.transform.translate.z = Random::Get() + newTransform.z;

    Random::SetMinMax(-rotateOffset, rotateOffset);
    particle.transform.rotate = Vector3{ Random::Get(), Random::Get(), Random::Get() } + transform.rotate_;

    particle.currentTime = 0;
    particle.color = color;

    return particle;
}


SphericalMove MakeNewSphericalCoordinate(const float& radius, const int& count, const int& maxCount, const float& radiusSpeed, const float& polarSpeed, const MinMax& polarSpeedMinMax, const MinMax& radiusSpeedMinMax)
{
    SphericalMove spherical;
    spherical.coordinate.azimuthal = 0.0f;
    spherical.coordinate.polar = std::numbers::pi_v<float>*2.0f / maxCount * count;
    spherical.coordinate.radius = radius;
    Random::SetMinMax(polarSpeedMinMax.min, polarSpeedMinMax.max);
    spherical.polarSpeed = polarSpeed + Random::Get();
    Random::SetMinMax(radiusSpeedMinMax.min, radiusSpeedMinMax.max);
    spherical.radiusSpeed = radiusSpeed + Random::Get();
    return spherical;
}

void ParticleManager::CreateParticleGroup(const std::string name, const Texture::TEXTURE_HANDLE& textureHandle, const bool& useModel, const ModelManager::MODEL_HANDLE& modelHandle)
{

    assert(!particleGroups.contains(name));
    std::unique_ptr<ParticleGroup> newParticleGroup = std::make_unique<ParticleGroup>();

    InitAccelerationField(*newParticleGroup);

    newParticleGroup->useBillboard = true;
    newParticleGroup->useSpriteCamera = false;

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

    for (auto& [name, group] : particleGroups) {

        if (group->useSpriteCamera) {
            //ビルボードをしない
            group->useBillboard = false;
        }

        UpdateBillBordMatrix(camera, *group);

        UpdateFunctions[group->movement](*group); // ← それぞれの動きに応じて更新！
    }
}

std::list<Particle> EmitParticles(const MinMax& velocityMinMax, const WorldTransform& transform, uint32_t count, const Vector4& color, const float& lifeTime, const AABB& translateAABB, const float& rotateOffset, const float& scaleOffset)
{
    std::list<Particle>particles;
    for (uint32_t i = 0; i < count; ++i) {
        particles.push_back(MakeNewParticle(velocityMinMax,transform, color, lifeTime, translateAABB, rotateOffset, scaleOffset));
    }
    return particles;
}


std::list<SphericalMove> EmitCoordinate(uint32_t count ,const float& radius, const float& radiusSpeed, const float& polarSpeed, const MinMax& polarSpeedMinMax, const MinMax& radiusSpeedMinMax)
{
    std::list<SphericalMove>sphericalCoordinates;

    for (uint32_t i = 0; i < count; ++i) {
        sphericalCoordinates.push_back(MakeNewSphericalCoordinate(radius, i, count, radiusSpeed, polarSpeed, polarSpeedMinMax, radiusSpeedMinMax));
    }
    return sphericalCoordinates;

}


void ParticleManager::Emit(Emitter& emitter)
{
    assert(particleGroups.contains(emitter.name));

    particleGroups[emitter.name]->particles.splice(particleGroups[emitter.name]->particles.end(), EmitParticles(emitter.velocityMinMax,emitter.transform, emitter.count, emitter.color, emitter.lifeTime, emitter.translateAABB_, emitter.rotateOffset_, emitter.scaleOffset_));

    particleGroups[emitter.name]->movement = emitter.movement;
    particleGroups[emitter.name]->parentPos_ = &emitter.transform;

    if (emitter.movement == kParticleSphere|| emitter.movement == kParticleShock) {
        particleGroups[emitter.name]->sphericalCoordinates.splice(particleGroups[emitter.name]->sphericalCoordinates.end(), EmitCoordinate(emitter.count, emitter.radius, emitter.radiusSpeed, emitter.polarSpeed, emitter.polarSpeedMinMax, emitter.radiusSpeedMinMax));
    }
}


void ParticleManager::Normal(ParticleGroup& group)
{
    group.numInstance = 0;

    for (std::list <Particle>::iterator particleIterator = group.particles.begin(); particleIterator != group.particles.end();) {

        if (group.numInstance < kNumMaxInstance) {

            //寿命に達していたらグループから外す
            if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
                particleIterator = group.particles.erase(particleIterator);
                continue;
            }

            //場の処理
            IsCollisionFieldArea(*particleIterator, group);
            //移動処理
            (*particleIterator).transform.translate += (*particleIterator).velocity * InverseFPS;
            //経過時間を加算
            (*particleIterator).currentTime += InverseFPS;

            UpdateMatrix(*particleIterator, group);

            //ビュープロジェクション行列
            UpdateWVPMatrix(*camera_, group);

            //データの更新
            UpdateInstancingData(group, *particleIterator);

        }
        ++particleIterator;
    }


}

void ParticleManager::Sphere(ParticleGroup& group)
{


    group.numInstance = 0;
    auto particleIterator = group.particles.begin();
    auto coordIterator = group.sphericalCoordinates.begin();

    while (particleIterator != group.particles.end() && coordIterator != group.sphericalCoordinates.end()) {

        if (group.numInstance < kNumMaxInstance) {

            if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
                particleIterator = group.particles.erase(particleIterator);
                coordIterator = group.sphericalCoordinates.erase(coordIterator);
                continue;
            }

            coordIterator->coordinate.polar += coordIterator->polarSpeed;

            if (coordIterator->coordinate.radius > 0.0f) {
                coordIterator->coordinate.radius -= coordIterator->radiusSpeed;
            } else {
                particleIterator = group.particles.erase(particleIterator);
                coordIterator = group.sphericalCoordinates.erase(coordIterator);
                continue;
            }

            Vector3 sphereCoordinate = TransformCoordinate(coordIterator->coordinate);

            particleIterator->transform.translate = group.parentPos_->GetWorldPosition() + particleIterator->velocity + sphereCoordinate;

            (*particleIterator).currentTime += InverseFPS;

            IsCollisionFieldArea(*particleIterator, group);

            UpdateMatrix(*particleIterator, group);

            UpdateWVPMatrix(*camera_, group);

            UpdateInstancingData(group, *particleIterator);

        }

        ++particleIterator;
        ++coordIterator;
    }


}

void ParticleManager::Shock(ParticleGroup& group)
{

    group.numInstance = 0;
    auto particleIterator = group.particles.begin();
    auto coordIterator = group.sphericalCoordinates.begin();

    while (particleIterator != group.particles.end() && coordIterator != group.sphericalCoordinates.end()) {

        if (group.numInstance < kNumMaxInstance) {

            if ((*particleIterator).lifeTime <= (*particleIterator).currentTime) {
                particleIterator = group.particles.erase(particleIterator);
                coordIterator = group.sphericalCoordinates.erase(coordIterator);
                continue;
            }


            if (coordIterator->coordinate.radius > 8.0f) {
                particleIterator = group.particles.erase(particleIterator);
                coordIterator = group.sphericalCoordinates.erase(coordIterator);
                continue;
            } else {
                coordIterator->coordinate.radius += coordIterator->radiusSpeed;
            }

            Vector3 sphereCoordinate = TransformCoordinate(coordIterator->coordinate);

            particleIterator->transform.translate = group.parentPos_->GetWorldPosition() + particleIterator->velocity + sphereCoordinate;

            (*particleIterator).currentTime += InverseFPS;

            IsCollisionFieldArea(*particleIterator, group);

            UpdateMatrix(*particleIterator, group);

            UpdateWVPMatrix(*camera_, group);

            UpdateInstancingData(group, *particleIterator);

        }

        ++particleIterator;
        ++coordIterator;
    }


}


// ==========================================================================================================


void ParticleManager::IsCollisionFieldArea(Particle& particleItr, ParticleGroup& group)
{
    if (IsCollision(group.accelerationField.area, particleItr.transform.translate)) {
        particleItr.velocity += group.accelerationField.acceleration * InverseFPS;
    }
}


void ParticleManager::Draw(uint32_t blendMode)
{
    for (const auto& [name, group] : particleGroups) {

        if (group->numInstance > 0) {
            //rootSignatureの設定
            commandList_->SetGraphicsRootSignature(rootSignature_->GetRootSignature(RootSignature::PARTICLE));
            commandList_->SetPipelineState(PSO::GetGraphicsPipelineStateParticle(blendMode).Get());
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

void ParticleManager::InitAccelerationField(ParticleGroup& group)
{
    group.accelerationField.acceleration = { 0.0f,0.0f,0.0f };
    group.accelerationField.area.min = { -1.0f,-1.0f,-1.0f };
    group.accelerationField.area.max = { 1.0f,1.0f,1.0f };
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

void ParticleManager::Reset(const std::string& name)
{
    if (particleGroups.contains(name)) {
        auto& group = particleGroups[name];
        group->particles.clear();
        group->sphericalCoordinates.clear();
        group->numInstance = 0;
        group->instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&group->instancingData));
        for (uint32_t i = 0; i < kNumMaxInstance; ++i) {
            group->instancingData[i].WVP = MakeIdentity4x4();
            group->instancingData[i].World = MakeIdentity4x4();
            group->instancingData[i].color = Vector4{ 1.0f, 1.0f, 1.0f, 0.0f }; // アルファ0で非表示に
        }
        group->instancingResource->Unmap(0, nullptr);
    }
}


std::unordered_map<std::string, std::unique_ptr<ParticleGroup>>& ParticleManager::GetParticleGroups()
{
    return particleGroups;
}

// ==========================================================================================================

void ParticleManager::UpdateBillBordMatrix(Camera& camera, ParticleGroup& group)
{

    backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);

    billboardMatrix = Multiply(backToFrontMatrix, camera.worldMat_);
    billboardMatrix.m[3][0] = 0.0f;
    billboardMatrix.m[3][1] = 0.0f;
    billboardMatrix.m[3][2] = 0.0f;

}

void ParticleManager::UpdateWVPMatrix(Camera& camera, ParticleGroup& group)
{

    if (group.useSpriteCamera) {
        worldViewProjectionMatrix = Multiply(worldMatrix, SpriteCamera::GetViewProjectionMatrix());
    } else {
        worldViewProjectionMatrix = Multiply(worldMatrix, camera.GetViewProjectionMatrix());
    }

}

void ParticleManager::UpdateMatrix(Particle& particleItr, ParticleGroup& group)
{
    //ビルボード処理
    if (group.useBillboard) {
        UpdateWorldMatrixForBillBord(particleItr, group);
    } else {
        UpdateWorldMatrix(particleItr, group);
    }
}
void ParticleManager::UpdateWorldMatrixForBillBord(Particle& particleItr, ParticleGroup& group)
{
    Matrix4x4 scaleMatrix = MakeScaleMatrix(particleItr.transform.scale);
    Matrix4x4 translateMatrix = MakeTranslateMatrix(particleItr.transform.translate);
    Matrix4x4 rotateMatrix = MakeRotateXYZMatrix(particleItr.transform.rotate) * billboardMatrix;
    worldMatrix = scaleMatrix * rotateMatrix * translateMatrix;
}

void ParticleManager::UpdateWorldMatrix(Particle& particleItr, ParticleGroup& group)
{

    if (group.useSpriteCamera) {
        Vector3 translate = particleItr.transform.translate;
        translate = translate * group.textureSize;
        translate.x *= -1.0f;

        worldMatrix = MakeAffineMatrix(particleItr.transform.scale * group.textureSize, particleItr.transform.rotate, translate);
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