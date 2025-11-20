#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include <functional>
#include <unordered_map>
#include "Data/FloorData.h"

class Model;
class Camera;
enum LightMode;
class CubeMesh;

class FloorGameFloor
{
public:
	FloorGameFloor();
	~FloorGameFloor();
	void Initialize();
	void Update();
	void Draw(Camera& camera, const LightMode& lightType);

	void SwapFloorType(FloorType type);
	void SwapNextFloorType();

	Object3d body_;
	FloorType floorType_;

private:
	//std::unique_ptr<CubeMesh>cubeMesh_ = nullptr;
	std::unordered_map<FloorType, std::function<void()>> floorTypeUpdate_;
	std::unordered_map<FloorType, Model*> models_;
	void NormalFloorUpdate();
	void StickyFloorUpdate();
	void StrongFloorUpdate();
	float autoSwapTimer_;
	float autoSwapDuration_;
	FloorType nextFloorType_;

};