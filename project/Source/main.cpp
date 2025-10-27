#include"MyEngine.h"
#include"GameScene.h"
#include"TitleScene.h"
#include"GameClearScene.h"
#include"GameOverScene.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

// Windowsアプリでのエントリーポイント(main関数)

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    D3DResourceLeakChecker leakCheck = {};
    // ==============================================//↓基本いじらない↓//============================================
    //エンジンの生成
    std::unique_ptr<MyEngine> myEngine = std::make_unique<MyEngine>();
    myEngine->Create(L"2102_ぶっとびミックス", WIN_WIDTH, WIN_HEIGHT);

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
    };

    const char* sceneName[] = {
       "TitleScene",
       "GameScene",
       "GameClearScene",
       "GameOverScene",
    };

    std::vector<std::unique_ptr<SceneManager>> scenes;
    // タイトルシーンの生成
    scenes.push_back(std::make_unique < TitleScene>());
    // ゲームシーンのインスタンスの取得
    scenes.push_back(std::make_unique < GameScene>());
    // ゲームクリアシーンの生成
    scenes.push_back(std::make_unique < GameClearScene>());
    // ゲームオーバーシーンの生成
    scenes.push_back(std::make_unique<GameOverScene>());

    //シーンのインデックス
    int sceneIndex = kTitleScene;
#ifdef _DEBUG
	// シーンのインデックス
	sceneIndex = kGameScene;
#endif // _DEBUG

	// 現在のシーン
	SceneManager* currentScene = nullptr;
	// 現在のシーンに代入
	currentScene = scenes[sceneIndex].get();
	// 現在のシーンの初期化
	currentScene->Initialize();
	for (int i = 0; i < scenes.size(); i++) {
		scenes[i]->SetIsEndScene(false);
	}

	// =============================================
	// ウィンドウのxボタンが押されるまでループ メインループ
	// =============================================
	while (true) {

		// メッセージが来たらループを抜ける
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

		if (scenes[kTitleScene]->GetIsEndScene()) {

			currentScene->SetIsEndScene(false);
	
			currentScene = scenes[kGameScene].get();
			// 現在のシーンの初期化
			currentScene->Initialize();
		}

		if (scenes[kGameScene]->GetIsEndScene()) {

			currentScene->SetIsEndScene(false);

			GameScene* gameScene = dynamic_cast<GameScene*>(scenes[kGameScene].get());

			if (gameScene->GetIsGameClear()) {
				GameClearScene* gameClearScene = dynamic_cast<GameClearScene*>(scenes[kGameClearScene].get());
				gameClearScene->SetScore(gameScene->GetScoreClass());
				gameClearScene->SetShutter(gameScene->GetShutter());
				currentScene = gameClearScene;

			} else if (gameScene->GetIsGameOver()) {
				GameOverScene* gameOverScene = dynamic_cast<GameOverScene*>(scenes[kGameOverScene].get());
				gameOverScene->SetScore(gameScene->GetScoreClass());
				gameOverScene->SetShutter(gameScene->GetShutter());
				currentScene = gameOverScene;
			}
			// 現在のシーンの初期化
			currentScene->Initialize();
		}

		if (scenes[kGameClearScene]->GetIsEndScene() || scenes[kGameOverScene]->GetIsEndScene()) {
			currentScene->SetIsEndScene(false);
			currentScene = scenes[kTitleScene].get();
			// 現在のシーンの初期化
			currentScene->Initialize();
		}

		// エンジンの更新処理
		myEngine->Update();

#ifdef _DEBUG
		// デバック用

		DebugUI::CheckColor(screenColor, "screenColor");
		ImGui::Text("%s", sceneName[sceneIndex]);
		DebugUI::CheckFPS();
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
