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
	uint32_t EmitSquare(const Vector3& position, const Vector2& dir, const Vector2& size, float duration);
	uint32_t EmitSquareForm(const Vector3& position, const Vector2& dir, const Vector2& size, float duration);

	void DeleteEffect(uint32_t effectID);

	void RotateEffect(uint32_t effectID, const Vector3& position, const Vector2& dir, const Vector2& size);
	void ZmRotateEffect(uint32_t effectID, const Vector3& position, const Vector2& dir, const Vector2& size);

private:
	AttackAreaEmitter();
	~AttackAreaEmitter() = default;

	static const int maxEffectNum_ = 20;
	std::array<std::unique_ptr<AttackAreaEffect>, maxEffectNum_> effectsPtr_;
};