#include "GameOverScene.h"

void GameOverScene::Initialize() {
    isEndScene_ = false;
}
void GameOverScene::Update() { isEndScene_ = true; }
void GameOverScene::Draw() {}
bool GameOverScene::GetIsEndScene()
{
    return isEndScene_;
}


