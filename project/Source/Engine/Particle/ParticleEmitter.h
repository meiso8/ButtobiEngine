#pragma once
#include"WorldTransform.h"
#include<cstdint>
#include<string>
#include"Vector4.h"
#include"BlendMode.h"
#include"Particle.h"

class Camera;

struct Emitter
{
    std::string name = "unknown";
    WorldTransform transform;//エミッタのTransfrom
    uint32_t count;//発生数
    float frequency;//発生頻度
    float frequencyTime;//頻度用時刻
    bool isRandom;
    Vector4 color;
    BlendMode blendMode;
    ParticleManager::Movements movement;
    float lifeTime;
    float radius;
};

class ParticleEmitter
{
private:
    ParticleManager* particleManager_ = nullptr;
public:
    Emitter emitter_{};
public:
    ParticleEmitter();
    void Initialize();
    void Update(Camera& camera);
    void Emit();
    void Draw();
    void SetName(const std::string name) { emitter_.name = name ; }
    void SetMovement(ParticleManager::Movements& movement) { emitter_.movement; }
};

