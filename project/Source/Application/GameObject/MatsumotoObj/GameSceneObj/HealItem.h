#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include"Collider.h"
#include"CharacterState.h"
class Model;
class Camera;
enum LightMode;


class HealItem : public Collider
{
public:
	HealItem();
	~HealItem();
	void OnCollision(Collider* collider)override;
	Vector3 GetWorldPosition() const override {
		return body_.worldTransform_.GetWorldPosition();
	}
	void Initialize();
	void Update();
	void Draw(Camera& camera, const LightMode& lightType);
	void Spawn(const Vector3& position);
	bool isActive_;
	Object3d body_;

private:
	Model* model_;
	float rotateSpeed_ = 1.0f;
};