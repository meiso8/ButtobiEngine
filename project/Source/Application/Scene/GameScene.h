#pragma once
#include"SceneManager.h"
#include "../CameraController.h"

#include"Particle/ChargeParticle.h"
#include"Particle/FlashParticle.h"
#include"Particle/AppleCrashParticle.h"
#include "Enemy.h"
#include "../UIManager.h"
#include "Player.h"
#include "Skydome.h"
#include "../Stage.h"
#include <vector>
#include<list>
#include"Arrow.h"

struct Plane;
struct OBB;

class CollisionManager;
class PlaneRenderer;
class OBBRenderer;
class Shutter;

// ゲームシーン
class GameScene :public SceneManager {

public:
    /// @brief コンストラクタ
    GameScene();

    /// @brief デフォルトデストラクタ
    ~GameScene() override;

    // 初期化
    void Initialize()override;

    // 更新
    void Update()override;

    // 描画
    void Draw()override;
    bool GetIsGameOver();
    bool GetIsGameClear();

    void Debug()override;

private:
    // カメラ操作
    std::unique_ptr <CameraController> cameraController_ = nullptr;
    bool isShakeCamera_ = false;
    uint32_t cameraShakeTimer_ = 0;

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

    //パーティクル
    std::unique_ptr <ChargeParticle> particle_ = nullptr;
    Vector4 chargeParticleColor_;
    std::unique_ptr <FlashParticle> flashParticle_ = nullptr;
    std::unique_ptr <AppleCrashParticle> crashParticle_ = nullptr;

    // 地形
    std::unique_ptr <Stage> stage_ = nullptr;
    std::unique_ptr <Arrow> forceArrow_ = nullptr;

    // スコア
    uint32_t score_ = 0;

    // UIマネージャー
    std::unique_ptr <UIManager> uiManager_ = nullptr;

    bool isGameOver = false;
    bool isGameClear = false;

    //シャッター
    std::unique_ptr< Shutter> shutter_ = nullptr;
    //アナウンスしたかどうかのフラグ
    bool isAnnounce_ = false;

    // 全ての当たり判定を行う
    void CheckAllCollisions();

    /// @brief 敵の出現
    void PopEnemy();

    void UpdateCamera();
    void CreateParticleMesh();
    void UpdateParticle();
    void UpdateSceneChange();
};