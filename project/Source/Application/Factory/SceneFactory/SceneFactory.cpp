#include"SceneFactory.h"
#include "FreeTypeScene/FreeTypeScene.h"
#include"GameScene/GameScene.h"
#include"TitleScene/TitleScene.h"
#include"ResultScene/ResultScene.h"
#include"FirstStoryScene/FirstStoryScene.h"
#include"SecondStoryScene/SecondStoryScene.h"

#include"SceneManager.h"
#include"Log.h"

void SceneFactory::Create()
{
    SceneManager::SetMap("FreeType", std::move(std::make_unique < FreeTypeScene>()));
    SceneManager::SetMap("Title", std::move(std::make_unique < TitleScene>()));
    SceneManager::SetMap("Game", std::move(std::make_unique < GameScene>()));
    SceneManager::SetMap("Result", std::move(std::make_unique < ResultScene>()));
    SceneManager::SetMap("FirstStory", std::move(std::make_unique < FirstStoryScene>()));
    SceneManager::SetMap("SecondStory", std::move(std::make_unique < SecondStoryScene>()));
    LogFile::Log("Create Scene\n");

    SceneManager::SetNextScene("Title");
#ifdef _DEVELOP
    SceneManager::SetNextScene("SecondStory");
#endif
    SceneManager::InitScene();

    LogFile::Log("Init Scene\n");

}
