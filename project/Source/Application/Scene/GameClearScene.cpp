#include "GameClearScene.h"

void GameClearScene::Initialize() { isEndScene_ = false; }
void GameClearScene::Update() {

    isEndScene_ = true;

}
void GameClearScene::Draw() {}
bool GameClearScene::GetIsEndScene()
{
    return isEndScene_;
}
