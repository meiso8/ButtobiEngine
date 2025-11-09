#include "ParticleEmitter.h"
#include"Particle.h"


ParticleEmitter::ParticleEmitter()
{
    Initialize();
}
void ParticleEmitter::Initialize()
{
    isRandom_ = false;
    scale_ = { 1.0f,1.0f,1.0f };
    color = { 1.0f,1.0f,1.0f,1.0f };

    emitter_.cont = 3;
    emitter_.frequency = 0.5f;
    emitter_.frequencyTime = 0.0f;
    emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
    emitter_.transform.scale = { 1.0f,1.0f,1.0f };
    emitter_.transform.translate = { 0.0f,0.0f,0.0f };
}
void ParticleEmitter::Update()
{
    emitter_.frequencyTime += ParticleManager::kDeltaTime;

    if (emitter_.frequency <= emitter_.frequencyTime) {
        emitter_.frequencyTime -= emitter_.frequency;
        Emit();
    }

}

void ParticleEmitter::Emit()
{
    ParticleManager::EmitParticle(name_, emitter_.transform.translate, emitter_.cont, scale_, color, isRandom_);
}
