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
#include"Camera/Camera.h"
#include"Camera/DebugCamera.h"

// ゲームシーン
class GameScene:public SceneManager {

public:
	// 初期化
	void Initialize()override;

	// 更新
	void Update()override;

	// 描画
	void Draw()override;

	void Debug()override;

	// デストラクタ
	~GameScene();

	// 全ての当たり判定を行う
	void CheckAllCollisions();

private:

	// カメラ
	Camera camera_;
	// カメラ操作
	CameraController* cameraController_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;
	// 自キャラモデル
	Model* playerModel_ = nullptr;
	// 敵キャラを複数用意
	std::list<Enemy*> enemies_;
	//  敵キャラモデル
	Model* enemyModel_ = nullptr;
	// 敵の発生させる数
	static inline const int kEnemyMax = 3;

	// 天球
	Skydome* skyDome_ = nullptr;
	// 天球モデル
	Model* skyDomeModel_ = nullptr;
	// 死亡時パーティクル
	Model* deathParticleModel_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;

	// 地形
	Stage* stage_ = nullptr;

	// UIマネージャー
	UIManager* uiManager_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラの生成
	DebugCamera* debugCamera_ = nullptr;

};