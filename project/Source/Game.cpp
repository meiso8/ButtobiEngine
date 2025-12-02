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

    scenes["Title"] = std::make_unique < TitleScene>();
    scenes["Sample"] = std::make_unique < SampleScene>();
 /*   scenes["Result"] = std::make_unique < ResultScene>();*/

    //最初の位置を保持
    currentIt = scenes.begin();
    currentScene = currentIt->second.get();
    currentScene->Initialize();
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

    currentScene->SceneChangeUpdate();

    if (currentScene->GetIsEndScene()) {
        ++currentIt; // 次のシーンへ

        if (currentIt == scenes.end()) {
            currentIt = scenes.begin(); // 最初に戻る
        }

        currentScene = currentIt->second.get();
        currentScene->Initialize();
    }

    // シーンの更新処理
    currentScene->Update();

}

void Game::Draw()
{
    // エンジンの描画前処理
    MyEngine::PreCommandSet();
    // シーンの描画
    currentScene->Draw();
    // エンジンの描画後処理
    MyEngine::PostCommandSet();
}

void Game::Debug()
{

#ifdef _DEBUG
    // デバック用
    MyEngine::Debug();

#ifdef USE_IMGUI

    for (const auto& [sceneName, scenePtr] : scenes) {
        if (scenePtr.get() == currentScene) {
            ImGui::Text("%s", sceneName.c_str());
            break;
        }
    }
#endif // USE_IMGUI

    currentScene->Debug();

#endif // _DEBUG

}
