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

std::list<Particle> Emit(const bool& isRandom, const Vector3& position, uint32_t count, const Vector3& scale = { 1.0f,1.0f,1.0f }, const Vector4& color = { 1.0f,1.0f,1.0f,1.0f });
Particle MakeNewParticle(const bool& isRandom, const Vector3& translate, const Vector3& scale = { 1.0f,1.0f,1.0f }, const Vector4& color = { 1.0f,1.0f,1.0f,1.0f });

class ParticleManager
{
public:

    AccelerationField accelerationField;
    bool useBillboard_ = true;
    const uint32_t kNumMaxInstance = 100;//インスタンス数
    static const float kDeltaTime;
protected:

    static ID3D12GraphicsCommandList* commandList_;
    static std::unordered_map<std::string, std::unique_ptr <ParticleGroup>>particleGroups;

    RootSignature* rootSignature_ = nullptr;

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

    ~ParticleManager();

    std::unordered_map<std::string, std::unique_ptr <ParticleGroup>>& GetParticleGroups() {
        return particleGroups;
    }


    void CreateParticleGroup(const std::string name, const uint32_t& textureHandle);
    void Create();

    virtual void Update(Camera& camera);

    static void EmitParticle(const std::string name, const Vector3& position, uint32_t count, const Vector3& scale = { 1.0f,1.0f,1.0f }, const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, const bool& isRandom = true);

    void Draw(uint32_t blendMode = BlendMode::kBlendModeAdd);

    void InitAccelerationField();

protected:
    void CreateModelData();
    void CreateVertexBufferResource();
    void UpdateBillBordMatrix(Camera& camera);
    void UpdateWorldMatrixForBillBord(Particle& particleItr);
    void UpdateWorldMatrix(Particle& particleItr);
    void IsCollisionFieldArea(Particle& particleItr);
};

