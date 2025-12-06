#pragma once
#include "MatsumotoObj/GameSceneObj/Data/MapData.h"
#include<unordered_map>

#include"Vector3.h"
typedef void(Pfunc)();

class Enemy;
class EnemyShockWaveManager;
class FloorGameFloorManager;
class EnemyShotWaveManager
{
public:

	enum Direction {
		LEFT,
		RIGHT,
		BACK,
	};

	EnemyShotWaveManager() = delete;
	EnemyShotWaveManager(Enemy* enemy, EnemyShockWaveManager* bombManager, FloorGameFloorManager* floorGameFloorManager);
	void Initialize();
	void Update();
	static Direction direction_;
private:
	Enemy* enemy_ = nullptr;
	EnemyShockWaveManager* shockWaveManager_ = nullptr;
	FloorGameFloorManager* floorGameFloorManager_ = nullptr;
	float shotSpeed_ = 0.3f;
	float shotSize_ = 0.5f;

	bool IsOccupied(int x, int y) {
		return used[x][y];
	}
	bool used[kMapWidth][kMapHeight] = {};

	void Left(int& randX, int& randY);
	void Right(int& randX, int& randY);
	void Back(int& randX, int& randY);
	static void (EnemyShotWaveManager::* spFuncTable[])(int& randX, int& randY);

	std::unordered_map<Direction, Vector3> enemyPoses_;
	Vector3 startPos_ = { 0.0f };
	Vector3 endPos_ = { 0.0f };
	float offset_ = 4.0f;
	float endOffset_ = 12.0f;
	float offsetBackStart_ = 2.0f;
	float enemyPosY_ = 4.0f;
};

