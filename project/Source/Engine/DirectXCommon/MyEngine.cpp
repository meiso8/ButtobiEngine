#include "MyEngine.h"
#include"SpriteCommon.h"
#include"TextureManager.h"
#include<algorithm>
#include"Camera/SpriteCamera.h"

PSO MyEngine::pso = {};
MyEngine* MyEngine::instance_ = nullptr;

DirectionalLight* MyEngine::directionalLightData = nullptr;
ModelConfig MyEngine::modelConfig_ = {};
std::unique_ptr<Window> MyEngine::wc = nullptr;

MyEngine* MyEngine::GetInstance()
{
    if (instance_ == nullptr) {
        instance_ = new MyEngine();
    }
    return instance_;
}

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
    input = Input::GetInstance();
    //入力
    input->Initialize(*wc);

    directXCommon = std::make_unique<DirectXCommon>();
    directXCommon.get()->Initialize(*wc);

    pso.CreateALLPSO();

    LogFile::Log("CreatePSO");

    //平行光源用のResourceを作成する
    directionalLightResource = DirectXCommon::CreateBufferResource(sizeof(DirectionalLight));
    //書き込むためのアドレスを取得
    directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
    //デフォルト値はとりあえず以下のようにしておく   
    directionalLightData->color = { 1.0f,1.0f,1.0f,1.0f };

    directionalLightData->direction = { 0.0f,-1.0f,0.0f };//向きは正規化する
    directionalLightData->intensity = 8.0f;
    //書き込み終了！
    directionalLightResource->Unmap(0, nullptr);

    modelConfig_.Initialize(PSO::rootSignature.get(), directionalLightResource.Get());

    //共通のスプライト
    SpriteCommon::Initialize();
    //スプライト用カメラ
    SpriteCamera::Initialize(static_cast<float>(wc->GetClientWidth()), static_cast<float>(wc->GetClientHeight()));
    //サウンド管理
    Sound::Initialize();
    //テクスチャ管理
    TextureManager::Initialize();

    ////ランダム関数の初期化
    //srand(static_cast<unsigned int>(time(nullptr)));



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

    TextureManager::Finalize();
    Sound::Finalize();
    directXCommon->EndFrame();
    wc->Finalize();
    //delete instance_;
}


//ここでBlenModeを変更する
void MyEngine::SetBlendMode(uint32_t blendMode,uint32_t cullMode) {

    DirectXCommon::GetCommandList()->SetPipelineState(pso.GetGraphicsPipelineState(blendMode, cullMode).Get());//PSOを設定

}