#include "MyEngine.h"
#include"SpriteCommon.h"
#include"TextureManager.h"
#include<algorithm>
#include"Camera/SpriteCamera.h"
#include"DrawGrid.h"

 std::unique_ptr<PSO> MyEngine::pso = nullptr;

std::unique_ptr <Input> MyEngine::input = nullptr;
DirectionalLight* MyEngine::directionalLightData = nullptr;
std::unique_ptr<ModelConfig> MyEngine::modelConfig_ = nullptr;
std::unique_ptr<Window> MyEngine::wc = nullptr;


std::unique_ptr<DirectXCommon> MyEngine::directXCommon = nullptr;
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

    pso = std::make_unique<PSO>();
    pso->CreateALLPSO();

    LogFile::Log("CreatePSO");

    //平行光源用のResourceを作成する
    directionalLightResource = DirectXCommon::CreateBufferResource(sizeof(DirectionalLight));
    //書き込むためのアドレスを取得
    directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
    //デフォルト値はとりあえず以下のようにしておく   
    directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };

    directionalLightData->direction = { 0.0f,0.0f,1.0f };//向きは正規化する
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
    TextureManager::Initialize();


    //音声の読み込み
    Sound::LoadAllSound();
    //テスクチャ読み込み
    Texture::LoadAllTexture();
    //モデル読み込み
    ModelManager::LoadAllModel();

#ifdef _DEBUG
    //グリット描画
    DrawGrid::Initialize();
#endif
    //ファイルへのログ出力
    LogFile::Log("LoopStart");

}

void MyEngine::Update() {

    //キーボード情報の取得開始
    input->Update();
    directXCommon->Update();

}

void MyEngine::PreCommandSet(Vector4& screenColor) {
    directXCommon->PreDraw(screenColor);
};

void MyEngine::PostCommandSet() {
    directXCommon->PostDraw();
};

void MyEngine::Finalize() {

    ModelManager::Finalize();

#ifdef _DEBUG
    //グリットを解放
    DrawGrid::Finalize();

#endif

    TextureManager::Finalize();
    Sound::Finalize();

    SpriteCommon::Finalize();

    modelConfig_.reset();

    if (directionalLightResource) {
        directionalLightResource.Reset();
    }

    pso.reset();

    directXCommon->EndFrame();
    directXCommon.reset();

    input.reset();

    wc->Finalize();
    wc.reset();

    logFile.reset();

}


//ここでBlenModeを変更する
void MyEngine::SetBlendMode(uint32_t blendMode,uint32_t cullMode) {

    DirectXCommon::GetCommandList()->SetPipelineState(pso->GetGraphicsPipelineState(blendMode, cullMode).Get());//PSOを設定

}