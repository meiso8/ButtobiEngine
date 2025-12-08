#pragma once
#include "AttackAreaEffect.h"
#include <memory>
#include <array>
#include <cstdint>

class AttackAreaEmitter {
public:
	static AttackAreaEmitter& GetInstance() {
		static AttackAreaEmitter instance;
		return instance;
	}

	void Initialize();
	void Finalize();
	void Update();
	void Draw(Camera& camera);
	uint32_t EmitCircle(const Vector3& position, float size, float duration);

	void DeleteEffect(uint32_t effectID);
private:
	AttackAreaEmitter();
	~AttackAreaEmitter() = default;

	static const int maxEffectNum_ = 20;
	std::array<std::unique_ptr<AttackAreaEffect>, maxEffectNum_> effectsPtr_;
};