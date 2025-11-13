#pragma once
#include"WorldTransform.h"
#include"Object3d.h"

class Model;
class Camera;
enum LightMode;
class CubeMesh;

class FloorGamePlayer
{
public:
	FloorGamePlayer();
	~FloorGamePlayer();

	void Initialize();
	void Update();
	void Draw(Camera& camera, const LightMode& lightType);

	Vector3& GetLookDir() { return lookDir_; }

	bool isStriptting_;
	bool isReqestStript_;
	bool isReqestShot_;
	Object3d body_;
private:
	void Move();
	void LookMoveDir();
	void StriptFloor();
	void ShotFloor();
	
	std::unique_ptr<CubeMesh>cubeMesh_ = nullptr;

	bool isMove_;
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