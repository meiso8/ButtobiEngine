#pragma once
#include"Sprite.h"
#include <memory>
class FloorGamePlayer;

class GameOverEvent final {
public:
	GameOverEvent() = delete;
	explicit GameOverEvent(FloorGamePlayer* player);
	~GameOverEvent();
	void Initialize();
	void Update();
	void Draw();
	bool IsGameOver() const { return isGameOver_; }
	bool IsRetrySelected() const { return isRetrySelected_; }
	bool IsReqestedAction() const { return isReqestedAction_; }
private:
	FloorGamePlayer* player_ = nullptr;
	bool isGameOver_ = false;
	float timer_ = 0.0f;
	const float gameOverDuration_ = 3.0f; // ゲームオーバーになるまでの時間（秒）

	bool isRetrySelected_ = true; // リトライが選択されているかどうか
	bool isReqestedAction_ = false; // 決定が押されたかどうか

	std::unique_ptr<Sprite> retrySprite_ = nullptr;
	std::unique_ptr<Sprite> backToTitleSprite_ = nullptr;
};
