#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include <functional>
#include <unordered_map>
#include <memory>

class Model;
class Camera;
enum LightMode;
class CircleMesh;

class ExprostionParticle
{
public:
	ExprostionParticle();
	~ExprostionParticle();
	void Initialize();
	void Update();
	void Draw(Camera& camera, const LightMode& lightType);
	void SetPosition(const Vector3& position);
	bool isActive_ = false;
	Object3d body_;
private:
	std::unordered_map<int, Model*> models_;
	float size_;
	float lifeTimer_;
	float lifeDuration_;
	std::unique_ptr<CircleMesh> circleMesh_ = nullptr;
};