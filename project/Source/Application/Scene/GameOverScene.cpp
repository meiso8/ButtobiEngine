#include "GameOverScene.h"

void GameOverScene::Initialize() {
	isEndGameOver_ = false; }
void GameOverScene::Update() { isEndGameOver_ = true; }
void GameOverScene::Draw() {}
bool GameOverScene::GetIsEndGameOver() { return isEndGameOver_; }