#pragma once
#include"Transform.h"
#include<cstdint>
#include<string>
#include"Vector4.h"
#include"BlendMode.h"
#include"Particle.h"
class Camera;

struct Emitter
{
    Transform transform;//エミッタのTransfrom
    uint32_t cont;//発生数
    float frequency;//発生頻度
    float frequencyTime;//頻度用時刻
    bool isRandom;
    Vector4 color;
    BlendMode blendMode;
    ParticleManager::Movements movement;
};

class ParticleEmitter
{
private:

    std::string name_ = "unknown";
    ParticleManager* particleManager_ = nullptr;
public:
    Emitter emitter_{};
public:

    void SetName(const std::string name) {
        name_ = name
            ;
    }

    ParticleEmitter();
    void Initialize();
    void Update(Camera& camera);
    void Emit();
    void Draw();
};

