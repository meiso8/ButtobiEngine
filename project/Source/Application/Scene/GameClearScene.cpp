#include "GameClearScene.h"

void GameClearScene::Initialize() { isEndGameClear_ = false; }
void GameClearScene::Update() {

isEndGameClear_ = true;

}
void GameClearScene::Draw() {}
bool GameClearScene::GetIsEndGameClear() { return isEndGameClear_; }