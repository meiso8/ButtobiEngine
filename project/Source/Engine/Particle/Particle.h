#pragma once
#include "ModelData.h"
#include"commandList.h"
#include"WorldTransform.h"
#include"MaterialResource.h"
#include"Transform.h"
#include"RootSignature.h"
#include "BlendState.h"
#include"AccelerationField.h"

#include<unordered_map>
#include<list>
#include<memory>
#include<cstdint>

class Camera;
class ShaderResourceView;

struct Emitter
{
    Transform transform;//エミッタのTransfrom
    uint32_t cont;//発生数
    float frequency;//発生頻度
    float frequencyTime;//頻度用時刻

};

struct Particle {
    Transform transform;
    Vector3 velocity;
    Vector4 color;

    float lifeTime;
    float currentTime;
};

struct ParticleForGPU {
    Matrix4x4 WVP;
    Matrix4x4 World;
    Vector4 color;
};

struct ParticleGroup {
    MaterialData materialData;
    std::list<Particle>particles;
    uint32_t instanceSrvIndex;
    Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
    uint32_t numInstance;//インスタンス数
    ParticleForGPU* instancingData;
};

std::list<Particle> Emit(const bool& isRandom, const Emitter& emitter, const Vector3& scale = { 1.0f,1.0f,1.0f }, const Vector4& color = { 1.0f,1.0f,1.0f,1.0f });
Particle MakeNewParticle(const bool& isRandom, const Vector3& translate, const Vector3& scale = { 1.0f,1.0f,1.0f }, const Vector4& color = { 1.0f,1.0f,1.0f,1.0f });

class ParticleManager
{
public:
    Emitter emitter_{};
    AccelerationField accelerationField;
    bool useBillboard_ = false;
    const uint32_t kNumMaxInstance = 100;//インスタンス数
protected:
    std::unordered_map<std::string, std::unique_ptr <ParticleGroup>>particleGroups;

    const float kDeltaTime = 1.0f / 60.0f;

    RootSignature* rootSignature_ = nullptr;
    static ID3D12GraphicsCommandList* commandList_;

    D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
    D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResource_;
    VertexData* vertexBufferData_ = nullptr;

    std::unique_ptr<ModelData> modelData_ = nullptr;

    std::unique_ptr < MaterialResource> materialResource;

    Matrix4x4 backToFrontMatrix;
    Matrix4x4 billboardMatrix;
    Matrix4x4 worldMatrix;
    Matrix4x4 worldViewProjectionMatrix;
public:

    std::unordered_map<std::string, std::unique_ptr <ParticleGroup>>& GetParticleGroups() {
        return particleGroups;
    }
    virtual ~ParticleManager();

    void CreateParticleGroup(const std::string name, const uint32_t& textureHandle);
    void Create();

    virtual void Update(Camera& camera);
    virtual void EmitParticle(const std::string name, const Vector3& position, uint32_t count, const Vector3& scale = {1.0f,1.0f,1.0f}, const Vector4& color = {1.0f,1.0f,1.0f,1.0f}, const bool& isRandom = true);
    void TimerUpdate(const std::string name, const Vector3& position, uint32_t count, const bool& isRandom, const Vector3& scale, const Vector4& color);

    void Draw(uint32_t blendMode = BlendMode::kBlendModeAdd);
    void Finalize();

    void InitEmitter();
    void InitAccelerationField();

protected:
    void CreateModelData();
    void CreateVertexBufferResource();
    void UpdateBillBordMatrix(Camera& camera);
    void UpdateWorldMatrixForBillBord(Particle& particleItr);
    void UpdateWorldMatrix(Particle& particleItr);
    void IsCollisionFieldArea(Particle& particleItr);
};

