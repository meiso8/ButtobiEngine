#pragma once
#include"WorldTransform.h"
#include"Object3d.h"

class Model;
class Camera;
enum LightMode;
class CubeMesh;
class FloorGamePlayer;

class FloorPlayerStripTargetUI
{
public:
	FloorPlayerStripTargetUI() = delete;
	FloorPlayerStripTargetUI(FloorGamePlayer* player);

	void Initialize();
	void Update();
	void Draw(Camera& camera, const LightMode& lightType);

	Object3d body_;
private:
	std::unique_ptr<CubeMesh>cubeMesh_ = nullptr;
	FloorGamePlayer* player_ = nullptr;
	bool isActive_ = false;
	float time_;
};