#pragma once
#include "FloorGameFloor.h"
#include <string>
#include <vector>
#include "Engine/Math/Vector/Vector2.h"

class CollisionManager;

class FloorGameFloorManager
{
public:
	FloorGameFloorManager();
	~FloorGameFloorManager();
	void Initialize();
	void Update();
	void Draw(class Camera& camera, const LightMode& lightType);

	void AddCollider(CollisionManager* collisionManager);
	FloorType GetFloorTypeAtPosition(const Vector3& position) const;
	std::vector<std::pair<int, int>> GetConnectedFloorsAtPosition(const Vector3& position) const;
	void SwapFloorTypeAtPosition(const int& xIndex, const int& yIndex);
	void SwapFloorTypeAtPosition(const Vector3& position);
	std::pair<int, int> GetFloorIndexAtPosition(const Vector3& position) const;
	//床の座標が欲しいのでここでゲッターを用意します
    Vector3& GetFloorPos(const int& xIndex, const int& yIndex);
	std::vector<Vector2> GetExprodedFloorMap() const;
private:
	std::vector<std::vector<std::unique_ptr<FloorGameFloor>>> floors_;
};