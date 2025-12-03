#pragma once
#include "ModelData.h"
#include"commandList.h"
#include"WorldTransform.h"
#include"MaterialResource.h"
#include"Transform.h"
#include"RootSignature.h"
#include "BlendState.h"
#include"AccelerationField.h"
#include"SphericalCoordinate.h"
#include"Texture.h"
#include"ModelManager.h"

#include<unordered_map>
#include<list>
#include<memory>
#include<cstdint>


class Camera;
class ShaderResourceView;
struct Emitter;

struct Particle {
    Transform transform;
    Vector3 velocity;
    Vector4 color;

    float lifeTime;
    float currentTime;
};

struct MinMax {
    float min;
    float max;
};

struct SphericalMove {
    SphericalCoordinate coordinate;
    float radiusSpeed;
    float polarSpeed;
};

struct ParticleForGPU {
    Matrix4x4 WVP;
    Matrix4x4 World;
    Vector4 color;
};


enum ParticleMovements {
    kParticleNormal,
    kParticleSphere,
    kParticleShock
};

struct ParticleGroup {
    MaterialData materialData;
    std::list<Particle>particles;
    std::list<SphericalMove>sphericalCoordinates;
    uint32_t instanceSrvIndex;
    Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
    uint32_t numInstance;//インスタンス数
    ParticleForGPU* instancingData;
    Vector3 textureSize;
    bool useModel;
    bool useBillboard = true;
    bool useSpriteCamera = false;
    Model* model = nullptr;
    const WorldTransform* parentPos_ = nullptr;
    ParticleMovements movement;
    AccelerationField accelerationField;
};

std::list<SphericalMove> EmitCoordinate(uint32_t count, const float& radius, const float& radiusSpeed, const float& polarSpeed, const MinMax& polarSpeedMinMax, const MinMax& radiusSpeedMinMax);

std::list<Particle> EmitParticles(const WorldTransform& transform, uint32_t count, const Vector4& color, const float& lifeTime, const AABB& translateAABB, const float& rotateOffset, const float& scaleOffset);
Particle MakeNewParticle(const WorldTransform& transform, const Vector4& color, const float& lifeTime, const float& translateOffset , const float& rotateOffset,const float& scaleOffset);

SphericalMove MakeNewSphericalCoordinate(const float& radius, const int& count, const int& maxCount, const float& radiusSpeed, const float& polarSpeed, const MinMax& polarSpeedMinMax, const MinMax& radiusSpeedMinMax);

class ParticleManager
{
public:
    const uint32_t kNumMaxInstance = 100;//インスタンス数
private:
    RootSignature* rootSignature_ = nullptr;
    static ID3D12GraphicsCommandList* commandList_;
    static std::unordered_map<std::string, std::unique_ptr <ParticleGroup>>particleGroups;

    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    Microsoft::WRL::ComPtr<ID3D12Resource> vertexBufferResource_;
    VertexData* vertexBufferData_ = nullptr;

    std::unique_ptr<ModelData> modelData_ = nullptr;

    std::unique_ptr < MaterialResource> materialResource;

    Matrix4x4 backToFrontMatrix;
    Matrix4x4 billboardMatrix;
    Matrix4x4 worldMatrix;
    Matrix4x4 worldViewProjectionMatrix;

    static ParticleManager* instance_;
    Camera* camera_ = nullptr;
public:

    void CreateAll();

    //コンストラク・タデストラクタの隠ぺい
    ParticleManager();
    ~ParticleManager() = default;
    //コピーコンストラクタの封印
    ParticleManager(ParticleManager&) = delete;
    //コピー代入演算子の封印
    ParticleManager& operator=(ParticleManager&) = delete;
    void Create();
    static ParticleManager* GetInstance();

static void Emit(Emitter& emitter);

    std::unordered_map<std::string, std::unique_ptr <ParticleGroup>>& GetParticleGroups();
    void CreateParticleGroup(const std::string name, const Texture::TEXTURE_HANDLE& textureHandle, const bool& useModel = false, const ModelManager::MODEL_HANDLE& modelHandle = ModelManager::MODEL_HANDLE::BOX);

    void Update(Camera& camera);
    void Draw(uint32_t blendMode = BlendMode::kBlendModeAdd);
    void InitAccelerationField(ParticleGroup& group);
    void Finalize();

protected:
    void UpdateBillBordMatrix(Camera& camera, ParticleGroup& group);
    void UpdateMatrix(Particle& particleItr, ParticleGroup& group);
private:
    //メンバ関数ポインタテーブル
    std::unordered_map<ParticleMovements, std::function<void(ParticleGroup&)>> UpdateFunctions;
    void Normal(ParticleGroup& group);
    void Sphere(ParticleGroup& group);
    void Shock(ParticleGroup& group);

    void CreateModelData();
    void CreateVertexBufferResource();

    void IsCollisionFieldArea(Particle& particleItr ,ParticleGroup& group);
    void UpdateWorldMatrixForBillBord(Particle& particleItr, ParticleGroup& group);
    void UpdateWorldMatrix(Particle& particleItr, ParticleGroup& group);
    void UpdateWVPMatrix(Camera& camera, ParticleGroup& group);
    void UpdateInstancingData(ParticleGroup& group, Particle& particleItr);

};

