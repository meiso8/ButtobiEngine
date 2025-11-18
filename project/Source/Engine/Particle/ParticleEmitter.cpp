#include "ParticleEmitter.h"

#include"Camera.h"

ParticleEmitter::ParticleEmitter()
{
    Initialize();
    particleManager_ = ParticleManager::GetInstance();
    particleManager_->Create();
    particleManager_->SetMovement(emitter_.movement);
}
void ParticleEmitter::Initialize()
{
    emitter_.cont = 3;
    emitter_.frequency = 0.5f;
    emitter_.frequencyTime = 0.0f;
    emitter_.transform.rotate = { 0.0f,0.0f,0.0f };
    emitter_.transform.scale = { 1.0f,1.0f,1.0f };
    emitter_.transform.translate = { 0.0f,0.0f,0.0f };

    emitter_.isRandom = true;
    emitter_.color = { 1.0f,1.0f,1.0f,1.0f };
    emitter_.blendMode = kBlendModeAdd;
    emitter_.movement = ParticleManager::kNormal;


}
void ParticleEmitter::Update(Camera& camera)
{
    emitter_.frequencyTime += ParticleManager::kDeltaTime;

    if (emitter_.frequency <= emitter_.frequencyTime) {
        emitter_.frequencyTime -= emitter_.frequency;
        Emit();
    }

    particleManager_->Update(camera);


   
}

void ParticleEmitter::Emit()
{
    particleManager_->EmitParticle(name_, emitter_.transform, emitter_.cont, emitter_.color, emitter_.isRandom);
}

void ParticleEmitter::Draw()
{
    particleManager_->Draw(emitter_.blendMode);
}
