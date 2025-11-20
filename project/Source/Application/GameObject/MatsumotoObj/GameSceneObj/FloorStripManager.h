#pragma once
#include <unordered_map>
#include <functional>
class FloorGamePlayer;
class FloorGameFloorManager;
class PlayerFloorStripManager;
enum class FloorType;

class FloorStripManager
{
public:
	FloorStripManager() = delete;
	FloorStripManager(FloorGamePlayer* player, FloorGameFloorManager* floorManager, PlayerFloorStripManager* playerStripManager);

	void Initialize();
	void Update();

private:
	FloorGamePlayer* player_ = nullptr;
	FloorGameFloorManager* floorManager_ = nullptr;
	PlayerFloorStripManager* playerStripManager_ = nullptr;

	std::unordered_map<FloorType, std::function<void()>> striptTypeAction_;
	void NormalStript();
	void StickyStript();
	void StrongStript();
};