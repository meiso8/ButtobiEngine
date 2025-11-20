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
	HPs* GetHpsPtr() { return &damageStruct_.hps; }
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

	std::vector<std::pair<int, int>> strippedFloorMap_;
	PlayerAnimationState animationState_ = PlayerAnimationState::Idle;
	bool isMove_;

private:
	void Move();
	void LookMoveDir();
	void StriptFloor();
	void ShotFloor();
	void HitAction();
	void Flashing();
	void SetBodyColor(const Vector4& color);

	Model* model_ = nullptr;
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

	float stickyFloorSlowRate_;

};