#pragma once
#include "SceneManager.h"

class Enemy;
class Stage;
class Sprite;

/// @brief ゲームオーバーシーン
class GameOverScene : public SceneManager {
private:
	std::unique_ptr<Enemy> enemy_;	// 敵キャラ
	std::unique_ptr<Stage> stage_;	// ステージ
	uint32_t score_ = 0;			// スコア
	uint32_t sceneChangeTimer_ = 60*10;//シーン遷移タイマー
	std::unique_ptr<Sprite> gameOverSprite_ = nullptr;

	/// @brief 全ての当たり判定を行う
	void CheckAllCollisions();

public:
	~GameOverScene() override;

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