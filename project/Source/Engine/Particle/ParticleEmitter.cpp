#include "ParticleEmitter.h"

#include"Camera.h"

ParticleEmitter::ParticleEmitter()
{
    Initialize();
    particleManager_ = ParticleManager::GetInstance();
    particleManager_->Create();
}
void ParticleEmitter::Initialize()
{
    emitter_.count = 3;
    emitter_.frequency = 0.5f;
    emitter_.frequencyTime = 0.0f;
    emitter_.transform.rotate_ = { 0.0f,0.0f,0.0f };
    emitter_.transform.scale_ = { 1.0f,1.0f,1.0f };
    emitter_.transform.translate_ = { 0.0f,0.0f,0.0f };

    emitter_.isRandom = true;
    emitter_.color = { 1.0f,1.0f,1.0f,1.0f };
    emitter_.blendMode = kBlendModeAdd;
    emitter_.movement = ParticleMovements::kParticleNormal;
    emitter_.lifeTime = -1.0f;
    emitter_.radius = 5.0f;
}
void ParticleEmitter::Update(Camera& camera)
{
    emitter_.frequencyTime += ParticleManager::kDeltaTime;

    if (emitter_.frequency <= emitter_.frequencyTime) {
        emitter_.frequencyTime -= emitter_.frequency;
        Emit();
    }

    WorldTransformUpdate(emitter_.transform);

    particleManager_->Update(camera);

}

void ParticleEmitter::Emit()
{
    particleManager_->Emit(emitter_);
}

void ParticleEmitter::Draw()
{
    particleManager_->Draw(emitter_.blendMode);
}

void ParticleEmitter::SetParent(WorldTransform& parent)
{
    emitter_.transform.Parent(parent);

}
