#pragma once
#include "ModelData.h"
#include"commandList.h"
#include"WorldTransform.h"
#include"MaterialResource.h"
#include"Transform.h"
#include"RootSignature.h"
#include "BlendState.h"
#include<list>
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

std::list<Particle> Emit(const Emitter& emitter);



struct ParticleForGPU {
    Matrix4x4 WVP;
    Matrix4x4 World;
    Vector4 color;
};

class ParticleMesh
{
public:
    const uint32_t kNumMaxInstance = 100;//インスタンス数
    std::list<Particle>particles;
    bool useBillboard_ = false;
    Emitter emitter_{};
    AccelerationField accelerationField;
private:

    const float kDeltaTime = 1.0f / 60.0f;

    ParticleForGPU* instancingData = nullptr;
    ModelData modelData_;
    MaterialResource materialResource_{};
    RootSignature* rootSignature_ = nullptr;

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

public:
    void Initialize(uint32_t textureHandle);
    static Particle MakeNewParticle(const Vector3& translate);
    void Draw(Camera& camera,uint32_t blendMode = BlendMode::kBlendModeAdd);
    void Update();

private:
    void CreateModelData();
    void CreateTransformationMatrix();
};

