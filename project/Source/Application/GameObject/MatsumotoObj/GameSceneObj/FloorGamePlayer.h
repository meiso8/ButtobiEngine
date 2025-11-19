#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include"Collider.h"
#include"CharacterState.h"
class Model;
class Camera;
enum LightMode;

enum class PlayerAnimationState
{
	Idle,
	Walk,
	Stript,
	Shot
};

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
	HPs* GetHpsPtr() { return &hps_; }
	bool isStriptting_;
	bool isReqestStript_;
	int stripFloorPosX_;
	int stripFloorPosY_;
	bool isReqestShot_;
	bool isOnStickyFloor_;
	bool isOnStripedFloor_;
	Object3d body_;
	Object3d rightArmObject_;
	Object3d leftArmObject_;
	Object3d rightLegObject_;
	Object3d leftLegObject_;

	PlayerAnimationState animationState_ = PlayerAnimationState::Idle;
	bool isMove_;

private:
	void Move();
	void LookMoveDir();
	void StriptFloor();
	void ShotFloor();
	
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

	float stickyFloorSlowRate_;

	HPs hps_ = { 100,100 };
};