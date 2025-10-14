#pragma once
#include "ModelData.h"
#include"commandList.h"
#include"WorldTransform.h"
#include"MaterialResource.h"
#include"Transform.h"
#include"RootSignature.h"
#include "BlendState.h"

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

private:
    const uint32_t kNumMaxInstance = 10;//インスタンス数
    ParticleForGPU* instancingData = nullptr;
    std::vector<Particle>particles;


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
public:
    void Initialize(uint32_t textureHandle);
    void Create();
    //void Update(Camera& camera);
    Particle MakeNewParticle();
    void Draw(Camera& camera,BlendMode blendMode = BlendMode::kBlendModeNormal);
private:
    void CreateModelData();
    void CreateTransformationMatrix();
};

