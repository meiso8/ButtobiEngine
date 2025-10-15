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

class ParticleMesh
{
public:
    const uint32_t kNumMaxInstance = 10;//インスタンス数
    std::list<Particle>particles;
    bool useBillboard_ = false;
private:
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
    void Create(uint32_t maxInstance);
    Particle MakeNewParticle();
    void Draw(Camera& camera,uint32_t blendMode = BlendMode::kBlendModeAdd);
private:
    void CreateModelData();
    void CreateTransformationMatrix();
};

