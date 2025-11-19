#include"MyEngine.h"
#include"SampleScene.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"ResultScene.h"
#include<map>
#include<unordered_map>

#include"MakeMatrix.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

// Windowsアプリでのエントリーポイント(main関数)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    D3DResourceLeakChecker leakCheck = {};
    // ==============================================//↓基本いじらない↓//============================================
    //エンジンの生成
    std::unique_ptr<MyEngine> myEngine = std::make_unique<MyEngine>();
    myEngine->Create(L"ぶっ飛びエンジン！", WIN_WIDTH, WIN_HEIGHT);
    // ==============================================//↑基本いじらない↑//============================================
    //画面の色
    Vector4 screenColor = { 0.75f,0.5f,0.5f,1.0f };

    // =============================================
    // シーンの生成
    // =============================================

    std::map<const std::string, std::unique_ptr<SceneManager>> scenes;
    scenes["Title"] = std::make_unique < TitleScene>();
    scenes["Sample"] = std::make_unique < SampleScene>();
    //scenes["Result"] = std::make_unique < ResultScene>();
    
  
    //最初の位置を保持
    auto currentIt = scenes.begin();
    SceneManager* currentScene = currentIt->second.get();
    currentScene->Initialize();


    Vector3 axis = Normalize({ 1.0f,1.0f,1.0f });
    float angle = 0.44f;

    Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);

    // =============================================
    // ウィンドウのxボタンが押されるまでループ メインループ
    // =============================================
    while (true) {

        // メッセージが来たらループを抜ける
        if (myEngine->GetWC().ProcessMassage()) {
            break;
        }

        currentScene->SceneChangeUpdate();

        if (currentScene->GetIsEndScene()) {
            ++currentIt; // 次のシーンへ

            if (currentIt == scenes.end()) {
                currentIt = scenes.begin(); // 最初に戻る
            }

            currentScene = currentIt->second.get();
            currentScene->Initialize();
        }

        // エンジンの更新処理
        myEngine->Update();
        //エスケープボタンを押したら終了
        if (Input::IsTriggerKey(DIK_ESCAPE)) { break; }

#ifdef _DEBUG
        // デバック用
        myEngine->Debug();

#ifdef USE_IMGUI
        DebugUI::CheckColor(screenColor, "screenColor");
        DebugUI::ShowMatrix4x4(rotateMatrix,"rotateMatrix");
        for (const auto& [sceneName, scenePtr] : scenes) { 
            if (scenePtr.get() == currentScene) {
                ImGui::Text("%s", sceneName.c_str());
                break;
            }
        }
     
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
