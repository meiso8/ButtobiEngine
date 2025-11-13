#include"MyEngine.h"
#include"SampleScene.h"
#include"GameScene.h"
#include"TitleScene.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

// Windowsアプリでのエントリーポイント(main関数)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    D3DResourceLeakChecker leakCheck = {};
    // ==============================================//↓基本いじらない↓//============================================
    //エンジンの生成
    std::unique_ptr<MyEngine> myEngine = std::make_unique<MyEngine>();
    myEngine->Create(L"2206_チーム制作二回目", WIN_WIDTH, WIN_HEIGHT);

    // ==============================================//↑基本いじらない↑//============================================

    //画面の色
    Vector4 screenColor = { 0.75f,0.75f,0.75f,1.0f };

    // =============================================
    // シーンの生成
    // =============================================

    enum Scene {
        kTitleScene,
        kGameScene,
        kSampleScene,
        kMaxScene,
    };

    const char* sceneName[] = {
       "TitleScene",
        "GameScene",
       "SampleScene",
    };

    std::vector<std::unique_ptr<SceneManager>> scenes;
    scenes.push_back(std::make_unique < TitleScene>());
    scenes.push_back(std::make_unique < GameScene>());
    scenes.push_back(std::make_unique < SampleScene>());

    //シーンのインデックス
    int sceneIndex = kTitleScene;

#ifdef _DEBUG
    sceneIndex = kGameScene;
#endif // _DEBUG

    // 現在のシーン
    SceneManager* currentScene = nullptr;
    // 現在のシーンに代入
    currentScene = scenes[sceneIndex].get();
    // 現在のシーンの初期化
    currentScene->Initialize();

    uint32_t lightType = 0;

    // =============================================
    // ウィンドウのxボタンが押されるまでループ メインループ
    // =============================================
    while (true) {

        // メッセージが来たらループを抜ける
        if (myEngine->GetWC().ProcessMassage()) {
            break;
        }

#ifdef _DEBUG

        if (Input::IsTriggerKey(DIK_I)) {
            // 現在のシーンの初期化
            sceneIndex++;
            sceneIndex %= kMaxScene;
            // 現在のシーンに代入
            currentScene = scenes[sceneIndex].get();
            currentScene->Initialize();
        }

#endif // _DEBUG

        // エンジンの更新処理
        myEngine->Update();
        //エスケープボタンを押したら終了
        if (Input::IsTriggerKey(DIK_ESCAPE)) { break; }

#ifdef _DEBUG
        // デバック用
        myEngine->Debug();

#ifdef USE_IMGUI
        DebugUI::CheckColor(screenColor, "screenColor");
        ImGui::Text("%s", sceneName[sceneIndex]);
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

    // エンジンの終了
    myEngine->Finalize();

    return 0;
}
