#pragma once
#include"SceneManager.h"
#include "../CameraController.h"
#include "DeathParticles.h"
#include"Particle/Particle.h"
#include "Enemy.h"
#include "../UIManager.h"
#include "Player.h"
#include "Skydome.h"
#include "../Stage.h"
#include <vector>
#include<list>

struct Plane;
struct OBB;

class CollisionManager;
class PlaneRenderer;
class OBBRenderer;

// ゲームシーン
class GameScene :public SceneManager {

public:
    /// @brief コンストラクタ
    GameScene();

	/// @brief デフォルトデストラクタ
    ~GameScene();

    // 初期化
    void Initialize()override;

    // 更新
    void Update()override;

    // 描画
    void Draw()override;
    bool GetIsEndScene()override;

    bool GetIsGameOver();
    bool GetIsGameClear();

    void Debug()override;

private:
    // カメラ操作
    std::unique_ptr <CameraController> cameraController_ = nullptr;
    // 自キャラ
    std::unique_ptr<Player> player_ = nullptr;
    // 敵キャラリスト
    std::list<std::unique_ptr<Enemy>> enemies_;

	// 敵出現待機フラグとタイマー
    bool isWaitingToPop_ = false;
    int32_t waitToPopTimer_ = 0;

    // 衝突マネージャ
    std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

    // 天球
    std::unique_ptr <Skydome> skyDome_ = nullptr;
    std::unique_ptr <DeathParticles>deathParticles_ = nullptr;

    //パーティクル
    ParticleMesh particle_;

    // 地形
    std::unique_ptr <Stage> stage_ = nullptr;

	// スコア
	uint32_t score_ = 0;

    // UIマネージャー
    std::unique_ptr <UIManager> uiManager_ = nullptr;

    bool isGameOver = false;
    bool isGameClear = false;

    // 全ての当たり判定を行う
    void CheckAllCollisions();

    /// @brief 敵の出現
    void PopEnemy();
};