#include"MyEngine.h"
#include"SampleScene.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"ResultScene.h"
#include<map>
#include<unordered_map>
#include"Lights/DirectionalLightManager.h"
#include"MakeMatrix.h"

#include "MatsumotoObj/SceneStaticClassManager.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

// * エンジンとInGameの間のラッパークラス勝手に追加しました(マツモト) * //
#include "MatsumotoObj/KeyBindConfig.h"
#include"UI/PauseScreen.h"//ポーズ中の処理を自分も追加しました吉田
// * ラッパークラスここまで * //

// Windowsアプリでのエントリーポイント(main関数)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    D3DResourceLeakChecker leakCheck = {};
    // ==============================================//↓基本いじらない↓//============================================
    //エンジンの生成
    std::unique_ptr<MyEngine> myEngine = std::make_unique<MyEngine>();
    myEngine->Create(L"2206_ベリベリベンジ！", WIN_WIDTH, WIN_HEIGHT);
    // ==============================================//↑基本いじらない↑//============================================
    //画面の色
    Vector4 screenColor = { 0.373f,0.804f,0.894f,1.0f };

	// * ラッパークラスここから * //
	// キーバインドの初期化
    KeyBindConfig::Instance();
	// * ラッパークラスここまで * //

    // 松本さんのシングルトンクラス初期化
	MatsumotoSceneStaticClass::Initialize();

    // =============================================
    // シーンの生成
    // =============================================
    std::map<const std::string, std::unique_ptr<SceneManager>> scenes;
    scenes["Title"] = std::make_unique < TitleScene>();
    scenes["Game"] = std::make_unique < GameScene>();
    scenes["Result"] = std::make_unique < ResultScene>();

    //最初の位置を保持
	std::string startSceneName = "Result";
	SceneManager* currentScene = scenes[startSceneName].get();
	auto currentIt = scenes.find(startSceneName);
    currentScene->Initialize();

    // =============================================
    // ウィンドウのxボタンが押されるまでループ メインループ
    // =============================================
    while (true) {

        // メッセージが来たらループを抜ける
        if (Window::ProcessMassage()) {
            break;
        }

        currentScene->SceneChangeUpdate();


		// 一定時間操作が無かったらタイトルへ戻る
		KeyBindConfig::Instance().Update();
        if (currentScene == scenes["Result"].get() || currentScene == scenes["Game"].get()&& !PauseScreen::isPause_) {
            if (KeyBindConfig::Instance().noControllTimer_ >= 30.0f) {
                currentIt = scenes.find("Title");
                currentScene = currentIt->second.get();
                currentScene->Initialize();
                KeyBindConfig::Instance().noControllTimer_ = 0.0f;
            }
        }

		// シーンが終了していたら次のシーンへ
        if (currentScene->GetIsEndScene()) {
            
            if (PauseScreen::isBackToTitle) {
                currentIt = scenes.find("Title");
                PauseScreen::isBackToTitle = false;
            } else {
                ++currentIt; // 次のシーンへ
                if (currentIt == scenes.end()) {
                    currentIt = scenes.begin(); // 最初に戻る
                }
            }
         
            currentScene = currentIt->second.get();
            currentScene->Initialize();
        }

        // エンジンの更新処理
        myEngine->Update();

#ifdef _DEBUG
        // デバック用
        myEngine->Debug();

#ifdef USE_IMGUI

        DebugUI::CheckColor(screenColor, "screenColor");
        for (const auto& [sceneName, scenePtr] : scenes) { 
            if (scenePtr.get() == currentScene) {
                ImGui::Text("%s", sceneName.c_str());
                break;
            }
        }

        ImGui::Begin("Command");
        ImGui::Text("I : SceneChange");
        ImGui::Text("R : Initialize");
        ImGui::Text("ESCAPE : WindowEnd");
        ImGui::End();

 
#endif // USE_IMGUI

        currentScene->Debug();

#endif // _DEBUG

        // シーンの更新処理
        currentScene->Update();

        // エンジンの描画前処理
        myEngine->PreCommandSet(screenColor);
        // シーンの描画
        currentScene->Draw();
        // エンジンの描画後処理
        myEngine->PostCommandSet();
    }

    // 松本さんのシングルトンクラス終了処理
    MatsumotoSceneStaticClass::Finalize();

    // エンジンの終了
    myEngine->Finalize();

    return 0;
}
