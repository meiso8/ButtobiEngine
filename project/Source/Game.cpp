#include "Game.h"
#include"SampleScene.h"
#include"TitleScene.h"
#include"ResultScene.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

void Game::Initialize()
{
    //エンジンの生成
    MyEngine::Create(L"LE2A_19_ヨシダ_トモカ", WIN_WIDTH, WIN_HEIGHT);

    // =============================================
    // シーンの生成
    // =============================================

    SceneManager::SetMap("Title",std::move(std::make_unique < TitleScene>()));
    SceneManager::SetMap("Sample",std::move(std::make_unique < SampleScene>()));

    SceneManager::SetItr("Title");
    SceneManager::InitScene();
}

void Game::Finalize()
{
    // エンジンの終了
    MyEngine::Finalize();
}

void Game::Update()
{
    // エンジンの更新処理
    MyEngine::Update();
}

void Game::Draw()
{
    // エンジンの描画前処理
    MyEngine::PreCommandSet();
    // エンジンの描画後処理
    MyEngine::PostCommandSet();
}

void Game::Debug()
{

#ifdef _DEBUG
    // デバック用
    MyEngine::Debug();
#endif // _DEBUG

}
