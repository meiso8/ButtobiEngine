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

	void SorryUpdate();
	void Draw(Camera& camera, const LightMode& lightType);

	Object3d body_;
	bool isAnimEnd_;

	enum Parts {
		BODY,
		WING_L,
		WING_R
	};

private:
	Object3d wingLPos_;
	Object3d wingRPos_;
	void Winging(const float& speed);

	float timer_ = 0.0f;
	//モデル
	std::unordered_map< Parts, Model*> models_;
};