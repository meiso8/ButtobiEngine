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

uint32_t AttackAreaEmitter::EmitCircle(const Vector3& position, float size, float duration)
{
	Vector3 pos = position;
	pos.y = 0.0f;
	for (int i = 0; i < maxEffectNum_; i++) {
		if (!effectsPtr_[i]->isActive_) {
			effectsPtr_[i]->SpawnCircle(pos, size, duration);
			return i;
		}
	}
	return UINT32_MAX; // 全て使用中
}

uint32_t AttackAreaEmitter::EmitSquare(const Vector3& position, const Vector2& dir, const Vector2& size, float duration)
{
	Vector3 pos = position;
	pos.y = 0.0f;
	for (int i = 0; i < maxEffectNum_; i++) {
		if (!effectsPtr_[i]->isActive_) {
			effectsPtr_[i]->SpawnSquare(pos, dir, size, duration);
			return i;
		}
	}
	return UINT32_MAX; // 全て使用中
}

void AttackAreaEmitter::DeleteEffect(uint32_t effectID)
{
	if (effectID >= maxEffectNum_)return;
	effectsPtr_[effectID]->isActive_ = false;
}

AttackAreaEmitter::AttackAreaEmitter()
{
	for (int i = 0; i < maxEffectNum_; i++) {
		effectsPtr_[i] = std::make_unique<AttackAreaEffect>();
	}
}
