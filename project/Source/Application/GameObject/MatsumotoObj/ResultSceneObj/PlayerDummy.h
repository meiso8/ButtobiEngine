#pragma once
#include"WorldTransform.h"
#include"Object3d.h"

class Model;
class Camera;
enum LightMode;

class PlayerDummy
{
public:
	PlayerDummy();
	~PlayerDummy();
	void Initialize();
	void Update();
	void Draw(Camera& camera);

	Object3d body_;
	Object3d headObject_;
	Object3d rightArmObject_;
	Object3d leftArmObject_;
	Object3d rightLegObject_;
	Object3d leftLegObject_;
private:
	Model* model_ = nullptr;
};