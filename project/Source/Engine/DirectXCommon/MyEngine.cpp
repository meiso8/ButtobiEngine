#include "MyEngine.h"
#include"SpriteCommon.h"
#include"Texture.h"
#include<algorithm>
#include"SpriteCamera.h"
#include"DrawGrid.h"
#include"Particle.h"
#include"JsonFile.h"

std::unique_ptr<PSO> MyEngine::pso = nullptr;

std::unique_ptr <Input> MyEngine::input = nullptr;
DirectionalLight* MyEngine::directionalLightData = nullptr;
std::unique_ptr<ModelConfig> MyEngine::modelConfig_ = nullptr;
std::unique_ptr<Window> MyEngine::wc = nullptr;


std::unique_ptr<DirectXCommon> MyEngine::directXCommon = nullptr;
std::unique_ptr<SrvManager> MyEngine::srvManager = nullptr;
std::unique_ptr<ParticleManager>  MyEngine::particleManager_ = nullptr;
std::unique_ptr<LogFile> MyEngine::logFile = nullptr;

std::unique_ptr<ModelConfig> modelConfig_ = nullptr;


Microsoft::WRL::ComPtr <ID3D12Resource> MyEngine::directionalLightResource = nullptr;

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

    //平行光源用のResourceを作成する
    directionalLightResource = DirectXCommon::CreateBufferResource(sizeof(DirectionalLight));
    //書き込むためのアドレスを取得
    directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
    //デフォルト値はとりあえず以下のようにしておく   
    directionalLightData->color = { 1.0f,230.0f / 255.0f,200.0f / 255.0f,1.0f };

    directionalLightData->direction = { 0.0f,-1.0f,0.0f };//向きは正規化する
    directionalLightData->intensity = 1.0f;
    //書き込み終了！
    directionalLightResource->Unmap(0, nullptr);

    modelConfig_ = std::make_unique<ModelConfig>();
    modelConfig_->Initialize(PSO::rootSignature.get(), directionalLightResource.Get());

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
}

void MyEngine::Debug()
{
#ifdef USE_IMGUI
    DebugUI::CheckJsonFile();
    DebugUI::CheckFPS();
    DebugUI::CheckInput(*input);
    DebugUI::CheckDirectionalLight();

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

    modelConfig_->Finalize();
    modelConfig_.reset();

    if (directionalLightResource) {
        directionalLightResource.Reset();
    }

    pso.reset();

#ifdef USE_IMGUI
    //ImGuiの終了処理 ゲームループが終わったら行う
    imGuiClass.ShutDown();
#endif

    directXCommon->EndFrame();
    directXCommon.reset();

    srvManager.reset();

    input.reset();

    wc->Finalize();
    wc.reset();

    logFile.reset();

}
