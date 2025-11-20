#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include"Collider.h"
#include"CharacterState.h"
class Model;
class Camera;
enum LightMode;

class FloorGamePlayer: public Collider
{
public:
	FloorGamePlayer();
	~FloorGamePlayer();
	void OnCollision(Collider* collider)override;
	Vector3 GetWorldPosition() const override {
		return body_.worldTransform_.GetWorldPosition();
	}
	void Initialize();
	void Update();
	void Draw(Camera& camera, const LightMode& lightType);

	Vector3& GetLookDir() { return lookDir_; }
	HPs* GetHpsPtr() { return &damageStruct_.hps; }
	bool isStriptting_;
	bool isReqestStript_;
	bool isReqestShot_;
	Object3d body_;
private:
	void Move();
	void LookMoveDir();
	void StriptFloor();
	void ShotFloor();

	Model* model_ = nullptr;
	bool isMove_;
	bool isHit_ = false;
	Damage damageStruct_;
	float moveAcceleration_;
	float moveSpeed_;
	Vector3 moveDir_;
	Vector3 moveLimitMax_;
	Vector3 moveLimitMin_;

	Vector3 lookDir_;
	float lookSpeed_;
	
	float striptTimer_;
	float striptDuration_;

	float shotTimer_;
	float shotDuration_;
};