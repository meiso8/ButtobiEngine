#pragma once
#include"SceneManager.h"
#include "../CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "../UIManager.h"
#include "Player.h"
#include "Skydome.h"
#include "../Stage.h"
#include <vector>
#include<list>

// ゲームシーン
class GameScene:public SceneManager {

public:
	// 初期化
	void Initialize()override;

	// 更新
	void Update()override;

	// 描画
	void Draw()override;

	bool GetIsGameOver();
	bool GetIsGameClear();

	void Debug()override;

	// デストラクタ
	~GameScene();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

private:
	// カメラ操作
	CameraController* cameraController_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;
	// 敵キャラを複数用意
	std::list<Enemy*> enemies_;
	// 敵の発生させる数
	static inline const int kEnemyMax = 3;
	// 天球
	Skydome* skyDome_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;

	// 地形
	Stage* stage_ = nullptr;

	// UIマネージャー
	UIManager* uiManager_ = nullptr;

	bool isGameOver = false;
	bool isGameClear = false;


};