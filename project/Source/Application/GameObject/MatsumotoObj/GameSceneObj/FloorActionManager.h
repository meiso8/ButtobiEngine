#pragma once
class FloorGameFloorManager;
class FloorGamePlayer;

class FloorActionManager
{
public:
	FloorActionManager() = delete;
	FloorActionManager(FloorGamePlayer* player, FloorGameFloorManager* floorManager);
	void Initialize();
	void Update();

private:
	FloorGamePlayer* player_;
	FloorGameFloorManager* floorManager_;
};