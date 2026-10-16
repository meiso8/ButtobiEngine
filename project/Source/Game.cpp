#include "Game.h"
#include"D3DResourceLeakChecker.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

void Game::Initialize()
{
    //エンジンの生成
    ButtobiEngine::Create(L"第Ⅲのめ", WIN_WIDTH, WIN_HEIGHT);

    // バックグラウンドでアセット読み込みスレッドを起動する
    isLoading_ = true;
    loadThread_ = std::thread(&Game::LoadAssets, this);

}

void Game::Finalize()
{
    // アプリ終了時にロードスレッドが残っていたら安全に終了を待つ
    if (loadThread_.joinable())
    {
        loadThread_.join();
    }

    // エンジンの終了
    ButtobiEngine::Finalize();
}

void Game::Update()
{

    ButtobiEngine::Update();
    // ロード中かどうかで処理を分ける
    if (isLoading_)
    {
        // ロード画面アニメーションなどの更新
        ButtobiEngine::UpdateEyeCatch();
        return;
    }

    ButtobiEngine::GameUpdate();

}

void Game::Draw()
{

    ButtobiEngine::PreCommandSet();

    if (isLoading_)
    {
        // 「Now Loading...」などのロード画面を描画
    } else
    {    // エンジンの描画前処理    
        ButtobiEngine::GameDraw();
    }
     // エンジンの描画前処理
        ButtobiEngine::PostCommandSet(!isLoading_);

}

void Game::Debug()
{
    // ロード中かどうかで処理を分ける
    if (isLoading_)
    {
        // ロード画面アニメーションなどの更新
        return;
    }

#ifdef USE_IMGUI
    // デバック用
    ButtobiEngine::Debug();
#endif // USE_IMGUI

}

void Game::LoadAssets()
{
    ButtobiEngine::Load();
    // ロード完了
    isLoading_ = false;
}
