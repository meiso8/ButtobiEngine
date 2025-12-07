#include "MatsumotoObj/GameSceneObj/Data/MapData.h"
#pragma once
class Enemy;
class EnemyBombManager;
class FloorGameFloorManager;
#include"Vector3.h"
class EnemyFloorChangeManager
{
public:
	EnemyFloorChangeManager() = delete;
	EnemyFloorChangeManager(Enemy* enemy, EnemyBombManager* bombManager, FloorGameFloorManager* floorGameFloorManager);
	void Initialize();
	void Update();
	void SelectFloor(int& randX, int& randY, Vector3& endPos);
private:
	Enemy* enemy_ = nullptr;
	EnemyBombManager* bombManager_ = nullptr;
	FloorGameFloorManager* floorGameFloorManager_ = nullptr;
	float shotSpeed_ = 0.3f;
	float shotSize_ = 0.5f;

	bool IsOccupied(int x, int y) {
		return used[x][y];
	}
	bool used[kMapWidth][kMapHeight] = {};

	bool isSelected_ = false;
};

