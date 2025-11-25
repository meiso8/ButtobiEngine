#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include <functional>
#include <unordered_map>
#include"Collider.h"

class Model;
class Camera;
enum LightMode;

class BossDummy
{
public:
	BossDummy();
	~BossDummy();
	void Initialize();
	void Update();
	void Draw(Camera& camera, const LightMode& lightType);

	Object3d body_;
	bool isAnimEnd_;
private:
	float timer_ = 0.0f;
	Model* model_ = nullptr;
};