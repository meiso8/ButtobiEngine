#pragma once
#include "SceneManager.h"
#include"Score.h"
class Enemy;
class Stage;
class Sprite;

class Shutter;

/// @brief ゲームオーバーシーン
class GameOverScene : public SceneManager {
private:
	std::unique_ptr<Enemy> enemy_;	// 敵キャラ
	std::unique_ptr<Stage> stage_;	// ステージ
	//シャッター
	Shutter* shutter_ = nullptr;
	// スコア
	Score* score_ = nullptr;
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
	void Debug()override;
	void SetScore(Score* score) { score_ = score; };
	void SetShutter(Shutter* shutter) { shutter_ = shutter; };
};