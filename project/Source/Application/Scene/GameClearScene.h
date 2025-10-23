#pragma once
#include "SceneManager.h"
#include <list>

class Enemy;
class Stage;
class CollisionManager;
class Sprite;

/// @brief ゲームクリアシーン
class GameClearScene : public SceneManager {
private:
    std::list<std::unique_ptr<Enemy>> enemies_;						// 敵キャラリスト
    std::unique_ptr<Stage> stage_;									// ステージ
    uint32_t score_ = 0;											// スコア
    bool isWaitingToPop_ = false;									// 敵出現待機フラグ
    int32_t waitToPopTimer_ = 0;									// 敵出現待機タイマー
    int32_t subtractWaitToPopTimer_ = 60;                           //現待機時間の減少数
    std::unique_ptr<CollisionManager> collisionManager_ = nullptr;	// 衝突マネージャ
    std::unique_ptr<Sprite> clearSprite_ = nullptr;	                // クリアスプライト
    bool isRenderSprite_ = false;
    /// @brief 全ての当たり判定を行う
    void CheckAllCollisions();

    /// @brief 敵の出現
    void PopEnemy();

public:
    /// @brief デフォルトコンストラクタ
    GameClearScene();

    /// @brief デフォルトデストラクタ
    ~GameClearScene() override;

    /// @brief 初期化
    void Initialize() override;

    /// @brief 更新
    void Update() override;

    /// @brief 描画
    void Draw() override;

    /// @brief シーンの切り替えフラグを取得
    /// @return シーンの切り替えフラグ
    bool GetIsEndScene() override;
};
