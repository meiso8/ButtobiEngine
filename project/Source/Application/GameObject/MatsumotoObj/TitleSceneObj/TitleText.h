#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include <functional>
#include <unordered_map>
#include"Collider.h"
#include "MatsumotoObj/GameSceneObj/Data/Force.h"

class Model;
class Camera;
enum LightMode;
class CubeMesh;

class TitleText :public Collider
{
public:
	TitleText();
	~TitleText();
	void OnCollision(Collider* collider)override;
	Vector3 GetWorldPosition() const override {
		return body_.worldTransform_.GetWorldPosition();
	}
	bool GetIsBreak() const { return isBreak_; }
	int& GetHitCount(){ return hitCount_; }
	int GetMaxHitCount() { return maxHitCount_; }
	void Initialize();
	void Update();
	void Draw(Camera& camera, const LightMode& lightType);
	Object3d body_;

private:
	//std::unique_ptr<CubeMesh>cubeMesh_ = nullptr;
	//
	float damageTimer_;

	float rotatePower_;
	Force force_;

	int hitCount_;
	const int maxHitCount_ = 10;
	bool isBreak_;

	float timer_;
	Vector3 pos_;

	Model* model_ = nullptr;
};