#pragma once
#include "ModelData.h"
#include"commandList.h"
#include"WorldTransform.h"
#include"MaterialResource.h"
#include"Transform.h"
#include"PSO/RootSignature.h"
#include "PSO/BlendState.h"
#include<list>
#include<memory>
#include<cstdint>
#include"AccelerationField.h"


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

std::list<Particle> Emit(const bool& isRandom, const Emitter& emitter, const Vector3& scale = { 1.0f,1.0f,1.0f }, const Vector4& color = { 1.0f,1.0f,1.0f,1.0f });
Particle MakeNewParticle(const bool& isRandom, const Vector3& translate, const Vector3& scale = { 1.0f,1.0f,1.0f }, const Vector4& color = { 1.0f,1.0f,1.0f,1.0f });

class ParticleManager
{
public:
    const uint32_t kNumMaxInstance = 100;//インスタンス数
    std::list<Particle>particles;
    Emitter emitter_{};
    AccelerationField accelerationField;
    bool useBillboard_ = false;
protected:

    const float kDeltaTime = 1.0f / 60.0f;


    uint32_t numInstance_ = 0;
    ParticleForGPU* instancingData = nullptr;
    std::unique_ptr<ModelData> modelData_ = nullptr;
    std::unique_ptr < MaterialResource> materialResource_ = nullptr;
    RootSignature* rootSignature_ = nullptr;
    static ID3D12GraphicsCommandList* commandList_;

    D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
    D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
    Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResource_;
    VertexData* vertexBufferData_ = nullptr;
    uint32_t textureHandle_ = 0;


    Matrix4x4 backToFrontMatrix;
    Matrix4x4 billboardMatrix;
    Matrix4x4 worldMatrix;
    Matrix4x4 worldViewProjectionMatrix;
public:
    virtual ~ParticleManager();
    void Create(uint32_t textureHandle, int modelHandle = -1);

    virtual void Update(Camera& camera);
    virtual void EmitParticle(const bool& isRandom, const Vector3& scale, const Vector4& color);
    void TimerUpdate(const bool& isRandom, const Vector3& scale, const Vector4& color);

    void Draw(uint32_t blendMode = BlendMode::kBlendModeAdd);
    void Finalize();


    void InitEmitter();
    void InitAccelerationField();

protected:
    void CreateModelData(const uint32_t& textureHandle, const int& modelHandle);
    void CreateVertexBufferResource();
    void CreateTransformationMatrix();
    void UpdateBillBordMatrix(Camera& camera);
    void UpdateWorldMatrixForBillBord(Particle& particleItr);
    void UpdateWorldMatrix(Particle& particleItr);
    void IsCollisionFieldArea(Particle& particleItr);
};

