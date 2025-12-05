#pragma once
#include <vector>
#include <memory>
#include "StrippedFloor.h"
class FloorGamePlayer;
enum class FloorType;

class PlayerFloorStripManager {
public:
	PlayerFloorStripManager(FloorGamePlayer* player);
	void Initialize();
	void Update();
	void Draw(class Camera& camera);

	void ActiveAllFloors();
	void DeactiveAllFloors();

	void StripSingleFloor(FloorType type);
	void StripMapFloor(const std::vector<std::pair<int, int>>& floorMap, FloorType type);

	std::vector<std::pair<int, int>> GetRotetedFloorMap(const std::vector<std::pair<int, int>>& floorMap);

	bool isActive_;
	bool isSingle_;
	Object3d floorPivot_;

private:
	FloorGamePlayer* player_;
	std::vector<std::vector<std::unique_ptr<StrippedFloor>>> floors_;
	std::unique_ptr<StrippedFloor> singleFloor_;
};

