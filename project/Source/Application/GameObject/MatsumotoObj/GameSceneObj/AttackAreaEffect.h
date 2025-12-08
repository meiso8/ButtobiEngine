#pragma once
#include"WorldTransform.h"
#include"Object3d.h"

#include <functional>
#include <unordered_map>

class Model;
class Camera;
enum LightMode;

enum class AttackAreaEffectType {
	CIRCLE,
	SQUARE
};

class AttackAreaEffect final {
public:
	AttackAreaEffect();
	~AttackAreaEffect();
	void Initialize();
	void Update();
	void Draw(Camera& camera);
	void SpawnCircle(const Vector3& position,float size, float duration);
	void SpawnSquare(const Vector3& position,const Vector2& dir, const Vector2& size, float duration);

	void RotateSquare(const Vector3& position, const Vector2& dir, const Vector2& size);

	bool isActive_ = false;

	Object3d body_;
	Object3d underBody_;
	Vector3 startPosition_ = { 0.0f,0.0f,0.0f };
private:
	
	float lifeTimer_ = 0.0f;
	float lifeDuration_ = 1.0f;
	float size_ = 1.0f;
	Vector2 squareSize_ = { 1.0f,1.0f };

	std::unordered_map < AttackAreaEffectType, std::function<void()>>updateFunc_;

	AttackAreaEffectType type_ = AttackAreaEffectType::CIRCLE;
	void CircleUpdate();
	void SquareUpdate();
};