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

struct ParticleForGPU {
    Matrix4x4 WVP;
    Matrix4x4 World;
    Vector4 color;
};

struct ParticleGroup {
    MaterialData materialData;
    std::list<Particle>particles;
    std::list<SphericalCoordinate>sphericalCoordinates;
    uint32_t instanceSrvIndex;
    Microsoft::WRL::ComPtr<ID3D12Resource> instancingResource;
    uint32_t numInstance;//インスタンス数
    ParticleForGPU* instancingData;
    Vector3 textureSize;
    bool useModel;
    Model* model = nullptr;
};



std::list<SphericalCoordinate> EmitCoordinate(const bool& isRandom, uint32_t count, const float& radius = 3.0f);

std::list<Particle> EmitParticles(const bool& isRandom, const WorldTransform& transform, uint32_t count, const Vector4& color = { 1.0f,1.0f,1.0f,1.0f },const float& lifeTime = -1.0f);
Particle MakeNewParticle(const bool& isRandom, const WorldTransform& transform, const Vector4& color, const float& lifeTime = -1.0f);
SphericalCoordinate MakeNewSphericalCoordinate(const bool& isRandom = true,const float& radius = 3.0f);
class ParticleManager
{
public:

    enum Movements {
        kNormal,
        kSphere,
        kShock
    };

    AccelerationField accelerationField;
    bool useBillboard_ = true;
    bool useSpriteCamera_ = false;

    const uint32_t kNumMaxInstance = 100;//インスタンス数
    static const float kDeltaTime;
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
    void CreateParticleGroup(const std::string name, const Texture::TEXTURE_HANDLE& textureHandle, const bool& useModel, const ModelManager::MODEL_HANDLE& modelHandle = ModelManager::MODEL_HANDLE::BOX);

    void Update(Camera& camera, Movements& movement);
    void Draw(uint32_t blendMode = BlendMode::kBlendModeAdd);
    void InitAccelerationField();
    void Finalize();

protected:
    void UpdateBillBordMatrix(Camera& camera);
    void UpdateMatrix(Particle& particleItr, ParticleGroup& group);
private:
    //メンバ関数ポインタテーブル
    std::unordered_map<Movements, std::function<void()>> UpdateFunctions;
    void Normal();
    void Sphere();
    void Shock();

    void CreateModelData();
    void CreateVertexBufferResource();

    void IsCollisionFieldArea(Particle& particleItr);
    void UpdateWorldMatrixForBillBord(Particle& particleItr);
    void UpdateWorldMatrix(Particle& particleItr, ParticleGroup& group);
    void UpdateWVPMatrix(Camera& camera);
    void UpdateInstancingData(ParticleGroup& group, Particle& particleItr);

};

