#include"MyEngine.h"
#include"SampleScene.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"GameClearScene.h"
#include"GameOverScene.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

//Windowsアプリでのエントリーポイント(main関数)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    D3DResourceLeakChecker leakCheck = {};
    // ==============================================//↓基本いじらない↓//============================================
    //エンジンの生成
    MyEngine* myEngine = MyEngine::GetInstance();
    myEngine->Create(L"2102_ぶっとびミックス", WIN_WIDTH, WIN_HEIGHT);
    //音声の読み込み
    Sound::LoadAllSound();
    //テスクチャ読み込み
    Texture::LoadAllTexture();
    //モデル読み込み
    ModelManager::LoadAllModel();
    // ==============================================//↑基本いじらない↑//============================================

    //画面の色
    Vector4 screenColor = { 0.6f,0.6f,0.6f,1.0f };

    // =============================================
    // シーンの生成
    // =============================================

    enum Scene {
        kTitleScene,
        kGameScene,
        kGameClearScene,
        kGameOverScene,
        kSampleScene,
    };

    const char* sceneName[] = {
       "TitleScene",
       "GameScene",
       "GameClearScene",
       "GameOverScene",
       "SampleScene"
    };

    std::vector<std::unique_ptr<SceneManager>> scenes;
    // タイトルシーンの生成
    scenes.push_back(std::make_unique<TitleScene>());
    // ゲームシーンのインスタンスの取得
    scenes.push_back(std::make_unique<GameScene>());
    // ゲームクリアシーンの生成
    scenes.push_back(std::make_unique<GameClearScene>());
    // ゲームオーバーシーンの生成
    scenes.push_back(std::make_unique<GameOverScene>());
    //サンプルシーンの生成
    scenes.push_back(std::make_unique<SampleScene>());

    //シーンのインデックス
    int sceneIndex = kGameScene;
#ifdef _DEBUG
    //シーンのインデックス
    sceneIndex = kTitleScene;
#endif // _DEBUG

    //現在のシーン
    SceneManager* currentScene = nullptr;
    // 現在のシーンに代入
    currentScene = scenes[sceneIndex].get();
    //現在のシーンの初期化
    currentScene->Initialize();

    // =============================================
    //ウィンドウのxボタンが押されるまでループ メインループ
    // =============================================
    while (true) {

        //メッセージが来たらループを抜ける
        if (myEngine->GetWC().ProcessMassage()) {
            break;
        }


#ifdef _DEBUG

        if (Input::IsTriggerKey(DIK_RETURN)) {
            sceneIndex = (sceneIndex + 1) % scenes.size(); // 次のシーンへ
            currentScene = scenes[sceneIndex].get();
            // 現在のシーンの初期化
            currentScene->Initialize();
        }

#endif // _DEBUG

        if (scenes[kTitleScene]->GetIsEndScene() || scenes[kGameScene]->GetIsEndScene()) {

            GameScene* gameScene = dynamic_cast<GameScene*>(scenes[kGameScene].get());

            if (scenes[kTitleScene]->GetIsEndScene()) {
                // 現在のシーンに代入
                currentScene = scenes[kGameScene].get();
            } else if (gameScene->GetIsGameClear()) {
                currentScene = scenes[kGameClearScene].get();
            } else if (gameScene->GetIsGameOver()) {
                currentScene = scenes[kGameOverScene].get();
            } else if (scenes[kGameClearScene]->GetIsEndScene() || scenes[kGameOverScene]->GetIsEndScene()) {
                currentScene = scenes[kTitleScene].get();
            }

            // 現在のシーンの初期化
            currentScene->Initialize();
        }


        //エンジンの更新処理
        myEngine->Update();

#ifdef _DEBUG
        //デバック用

        DebugUI::CheckColor(screenColor, "screenColor");
        ImGui::Text("%s", sceneName[sceneIndex]);
        DebugUI::CheckFPS();
        currentScene->Debug();

#endif // _DEBUG

        //シーンの更新処理
        currentScene->Update();

        //エンジンの描画前処理
        myEngine->PreCommandSet(screenColor);
        //シーンの描画
        currentScene->Draw();
        //エンジンの描画後処理
        myEngine->PostCommandSet();

    }

    //エンジンの終了
    myEngine->Finalize();

    return 0;

}
