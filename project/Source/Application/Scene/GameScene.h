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

    // デストラクタ
    ~GameScene();

    // 全ての当たり判定を行う
    void CheckAllCollisions();

private:
    // カメラ操作
    std::unique_ptr <CameraController> cameraController_ = nullptr;
    // 自キャラ
    std::unique_ptr<Player> player_ = nullptr;
    // 敵キャラを複数用意
    std::list<Enemy*> enemies_;

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

	// 平面
	std::array<std::unique_ptr<Plane>, 2> planes_;
	std::array<std::unique_ptr<PlaneRenderer>, 2> planeRenderers_;

	// OBB
	std::array<Vector3, 4> obbRotates_;
	std::array<std::unique_ptr<OBB>, 4> obbs_;
	std::array<std::unique_ptr<OBBRenderer>, 4> obbRenderers_;

    bool isGameOver = false;
    bool isGameClear = false;

    /// @brief 敵の出現
    void PopEnemy();
};