#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include <functional>
#include <unordered_map>

class Model;
class Camera;
enum LightMode;
enum class FloorType;

class StrippedFloor
{
public:
	StrippedFloor();
	~StrippedFloor();
	void Initialize();
	void Update();
	void Draw(Camera& camera, const LightMode& lightType);

	void SwapType(FloorType type) { requestType_ = type; }

	Object3d body_;
	bool isActive_;

private:
	std::unordered_map<FloorType, Model*> models_;
	FloorType type_;
	FloorType requestType_;
};