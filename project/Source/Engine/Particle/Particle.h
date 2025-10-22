#pragma once
#include "ModelData.h"
#include"commandList.h"
#include"WorldTransform.h"
#include"MaterialResource.h"
#include"Transform.h"
#include"RootSignature.h"
#include "BlendState.h"
#include<list>
#include<memory>
#include<cstdint>
#include"AccelerationField.h"
#include"SphericalCoordinate.h"

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

std::list<Particle> Emit(const Emitter& emitter, const Vector4& color);
std::list<SphericalCoordinate> EmitCoordinate(const Emitter& emitter);


class ParticleManager
{
public:
    const uint32_t kNumMaxInstance = 100;//インスタンス数
    std::list<Particle>particles;
    std::list<SphericalCoordinate>sphericalCoordinates;
    bool useBillboard_ = false;
    Emitter emitter_{};
    AccelerationField accelerationField;
private:

    const float kDeltaTime = 1.0f / 60.0f;


    uint32_t numInstance_ = 0;
    ParticleForGPU* instancingData = nullptr;
    std::unique_ptr<ModelData> modelData_ = nullptr;
    MaterialResource materialResource_{};
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

public:
    ~ParticleManager();
    void Initialize(uint32_t textureHandle, int modelHandle = -1);

    void InitEmitter();
    void InitAccelerationField();

    void Draw(uint32_t blendMode = BlendMode::kBlendModeAdd);
    void EmitterTimerUpdate(const Vector4 color);

    void Update(Camera& camera);

    void EmitParticle(const Vector4& color);
    static Particle MakeNewParticle(const Vector3& translate, const Vector4& color);
    static SphericalCoordinate MakeNewSphericalCoordinate();
private:
    void CreateModelData(const uint32_t& textureHandle,const int& modelHandle);
    void CreateVertexBufferResource();
    void CreateTransformationMatrix();

 

};

