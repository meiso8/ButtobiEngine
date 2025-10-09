#include"MyEngine.h"
#include"SampleScene.h"
#include"GameScene.h"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    //エンジンの生成
    MyEngine* myEngine = MyEngine::GetInstance();
    myEngine->Create(L"2102", WIN_WIDTH, WIN_HEIGHT);

    //音声の読み込み
    Sound::GetInstance()->Load();
    //テスクチャ読み込み
    Texture::GetInstance()->Load();
    //モデル読み込み
    ModelManager::LoadAllModel();
    //画面の色
    Vector4 screenColor = { 0.6f,0.6f,0.6f,1.0f };
    //サンプルシーンの生成
    std::unique_ptr<SampleScene> sampleScene = std::make_unique<SampleScene>();
    // ゲームシーンのインスタンスの取得
    std::unique_ptr<GameScene> gameScene = std::make_unique<GameScene>();

    //現在のシーン
    SceneManager* currentScene = nullptr;
    //現在のシーンに代入
    currentScene = gameScene.get();
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

        //エンジンの更新処理
        myEngine->Update();

#ifdef _DEBUG
        //デバック用
        currentScene->Debug();
        DebugUI::CheckColor(screenColor);
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
