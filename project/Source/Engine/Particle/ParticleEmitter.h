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
    float translateOffset_ = 0.0f;
    float rotateOffset_ = 0.0f;
    float scaleOffset_ = 0.0f;
    uint32_t count;//発生数
    float frequency;//発生頻度
    float frequencyTime;//頻度用時刻
    Vector4 color;//色
    BlendMode blendMode;//ブレンドモード
    ParticleMovements movement;//動き
    float lifeTime;//生存時間
    //球面座標の半径
    float radius;
    //半径の移動速度
    float radiusSpeed = 0.0f;
    MinMax radiusSpeedMinMax;
    //経度の移動速度
    float polarSpeed = 0.0f;
    MinMax polarSpeedMinMax;

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
    void UpdateTimer();
    void InitTimer();
    void Update(Camera& camera);
    void Emit();
    void Draw();
    void SetName(const std::string name) { emitter_.name = name; }
    void SetMovement(ParticleMovements& movement) { emitter_.movement; }
    void SetParent(WorldTransform& parent);
};

