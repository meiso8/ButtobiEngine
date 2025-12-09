#include "MyEngine.h"
#include"SpriteCommon.h"
#include"Texture.h"
#include<algorithm>
#include"SpriteCamera.h"
#include"DrawGrid.h"
#include"Particle.h"
#include"JsonFile.h"
#include"VibrateManager.h"
#include"Lights/PointLightManager.h"
#include"Lights/DirectionalLightManager.h"
#include"Lights/SpotLightManager.h"

std::unique_ptr<PSO> MyEngine::pso = nullptr;
std::unique_ptr <Input> MyEngine::input = nullptr;
std::unique_ptr<Window> MyEngine::wc = nullptr;

std::unique_ptr<DirectXCommon> MyEngine::directXCommon = nullptr;
std::unique_ptr<SrvManager> MyEngine::srvManager = nullptr;
std::unique_ptr<ParticleManager>  MyEngine::particleManager_ = nullptr;
std::unique_ptr<LogFile> MyEngine::logFile = nullptr;

void MyEngine::Create(const std::wstring& title, const int32_t clientWidth, const int32_t clientHeight) {


    //誰も捕捉しなかった場合に(Unhandled),補足する関数を登録
    //main関数始まってすぐに登録すると良い
    SetUnhandledExceptionFilter(ExportDump);

    logFile = std::make_unique<LogFile>();
    logFile->Create();
    LogFile::Log("CreateLogFile");

    //WindowClassの生成
    wc = std::make_unique<Window>();
    wc->Create(title, clientWidth, clientHeight);
    LogFile::Log("CreateWindowClass");

    //InputClassの生成
    input = std::make_unique<Input>();
    //入力
    input->Initialize(*wc);
    //コントローラー
    VibrateManager::Initialize();

    directXCommon = std::make_unique<DirectXCommon>();
    directXCommon->Initialize(*wc);

    srvManager = std::make_unique<SrvManager>();
    srvManager->Initialize();

#ifdef USE_IMGUI
    //ImGuiの初期化。
    imGuiClass.Initialize(*wc, directXCommon->GetDevice().Get(), directXCommon->GetSwapChain(), directXCommon->GetSwapChainRtv());
    LogFile::Log("InitImGui");
#endif
    pso = std::make_unique<PSO>();
    pso->CreateALLPSO();

    LogFile::Log("CreatePSO");


    DirectionalLightManager::Create();
    PointLightManager::CreateData();
    SpotLightManager::Create();
    ////共通のスプライト
    SpriteCommon::Initialize();
    //スプライト用カメラ
    SpriteCamera::Initialize(static_cast<float>(wc->GetClientWidth()), static_cast<float>(wc->GetClientHeight()));
    //サウンド管理
    Sound::Initialize();
    //テクスチャ管理
    Texture::Initialize();

    //テスクチャ読み込み
    Texture::LoadAllTexture();
    //音声の読み込み
    Sound::LoadAllSound();
    //モデル読み込み
    ModelManager::LoadAllModel();
    //JsonFileの読み込み
    JsonFile::LoadAllJsonFile();

#ifdef _DEBUG
    //グリット描画
    DrawGrid::Create();
#endif

    particleManager_ = std::make_unique <ParticleManager>();
    particleManager_->Create();
    particleManager_->CreateAll();
    //ファイルへのログ出力
    LogFile::Log("LoopStart");


}

void MyEngine::Update() {

    //キーボード情報の取得開始
    input->Update();
#ifdef USE_IMGUI
    //ImGuiにここからフレームが始まる旨を伝える
    imGuiClass.FrameStart();
#endif
    VibrateManager::Update();
}

void MyEngine::Debug()
{
#ifdef USE_IMGUI
    DebugUI::CheckJsonFile();
    DebugUI::CheckFPS();
    DebugUI::CheckInput();

    DebugUI::CheckSound();
    DebugUI::CheckDirectionalLight();
    DebugUI::CheckSpotLight();
    DebugUI::CheckPointLightData(PointLightManager::GetPointLightData(0),"pointLight0");
    DebugUI::CheckPointLightData(PointLightManager::GetPointLightData(1), "pointLight1");
    DebugUI::CheckPointLightData(PointLightManager::GetPointLightData(2), "pointLight2");
#endif // USE_IMGUI


}

void MyEngine::PreCommandSet(Vector4& screenColor) {

#ifdef USE_IMGUI
    //ImGuiの内部コマンドを生成する
    imGuiClass.Render();
#endif
    directXCommon->PreDraw(screenColor);
};

void MyEngine::PostCommandSet() {
#ifdef USE_IMGUI
    //諸々の描画処理が終了下タイミングでImGuiの描画コマンドを積む
    imGuiClass.DrawImGui(CommandList::GetCommandList().Get());

#endif // _DEBUG
    directXCommon->PostDraw();
};

void MyEngine::Finalize() {



    particleManager_->Finalize();
    particleManager_.reset();

    ModelManager::Finalize();

#ifdef _DEBUG
    //グリットを解放
    DrawGrid::Finalize();

#endif

    Texture::Finalize();
    Sound::Finalize();

    SpriteCommon::Finalize();

    SpotLightManager::Finalize();
    PointLightManager::Finalize();
    DirectionalLightManager::Finalize();

    pso.reset();

#ifdef USE_IMGUI
    //ImGuiの終了処理 ゲームループが終わったら行う
    imGuiClass.ShutDown();
#endif

    directXCommon->EndFrame();
    directXCommon.reset();

    srvManager.reset();

    VibrateManager::Finalize();
    input.reset();

    wc->Finalize();
    wc.reset();

    logFile.reset();

}
