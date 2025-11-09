#pragma once
#include"Transform.h"
#include<cstdint>
#include<string>
#include"Vector4.h"

struct Emitter
{
    Transform transform;//エミッタのTransfrom
    uint32_t cont;//発生数
    float frequency;//発生頻度
    float frequencyTime;//頻度用時刻

};

class ParticleEmitter
{
private:

    std::string name_ = "unknown";
    bool isRandom_ = false;
    Vector3 scale_ = { 1.0f,1.0f,1.0f };
    Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
public:
    Emitter emitter_{};
public:

    void SetName(const std::string name) {
        name_ = name
            ;
    }

    ParticleEmitter();
    void Initialize();
    void Update();
    void Emit();
};

