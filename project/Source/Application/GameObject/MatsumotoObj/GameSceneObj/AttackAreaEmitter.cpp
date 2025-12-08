#include "AttackAreaEmitter.h"

void AttackAreaEmitter::Initialize()
{
	for (int i = 0; i < maxEffectNum_; i++) {
		effectsPtr_[i]->Initialize();
	}
}

void AttackAreaEmitter::Finalize()
{
	for (int i = 0; i < maxEffectNum_; i++) {
		effectsPtr_[i].reset();
	}
}

void AttackAreaEmitter::Update()
{
	for (int i = 0; i < maxEffectNum_; i++) {
		effectsPtr_[i]->Update();
	}
}

void AttackAreaEmitter::Draw(Camera& camera)
{
	for (int i = 0; i < maxEffectNum_; i++) {
		effectsPtr_[i]->Draw(camera);
	}
}

void AttackAreaEmitter::EmitCircle(const Vector3& position, float size, float duration)
{
	for (int i = 0; i < maxEffectNum_; i++) {
		if (!effectsPtr_[i]->isActive_) {
			effectsPtr_[i]->SpawnCircle(position, size, duration);
			break;
		}
	}
}

AttackAreaEmitter::AttackAreaEmitter()
{
	for (int i = 0; i < maxEffectNum_; i++) {
		effectsPtr_[i] = std::make_unique<AttackAreaEffect>();
	}
}
