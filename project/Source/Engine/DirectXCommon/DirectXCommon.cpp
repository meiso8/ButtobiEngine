#include "DirectXCommon.h"
#include"Log.h"

#include<cassert>

#include"CbvSrvUavDescriptorHeap.h"
#include"RtvDescriptorHeap.h"
#include"DsvDescriptorHeap.h"

#include "PostProcessManager/PostProcessManager.h"

#include"DebugUI.h"
#include"Input.h"
#include"ResourceManager/ResourceManager.h"
#include"DepthTexture/DepthTexture.h"
#include"../CommandListManager/CommandListManager.h"

using namespace Microsoft::WRL;
ComPtr<ID3D12Device> DirectXCommon::device = nullptr;
std::unique_ptr< DxcCompiler> DirectXCommon::dxcCompiler = nullptr;

DirectXCommon::~DirectXCommon()
{

    renderTexture_->Clear();
    renderTextureForSprite_->Clear();
  

    if (device) {
        device.Reset();
  }
  
}

void DirectXCommon::Finalize()
{
    LogFile::Log("DirectXCommon Finalize\n");
}

void DirectXCommon::InitializeBarrier(CommandListManager& commandListManager)
{    //コマンドリストのセット
    barrierMain_.SetCommandList(commandListManager.GetCommandList(CommandListManager::kMain));
    barrierLoad_.SetCommandList(commandListManager.GetCommandList(CommandListManager::kLoad));
}
void DirectXCommon::PostInitialize()
{

    CreateDXCCompiler();

}

void DirectXCommon::RenderTexturePreDraw(CommandListManager& commandListManager,DsvDescriptorHeap* dsvDescriptorHeap, DepthTexture* depthTexture)
{

    auto* commandList = commandListManager.GetCommandList(CommandListManager::kMain);

    barrierMain_.SettingBarrier(
        depthTexture->GetResource(),
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_DEPTH_WRITE);

    auto& renderTextureDataNormal = renderTexture_->GetRenderTextureData(RenderTexture::kNormal0);
    auto& renderTextureDataThermography = renderTexture_->GetRenderTextureData(RenderTexture::kThermography);
    auto& renderTextureDataID = renderTexture_->GetRenderTextureData(RenderTexture::kObjectID);

    // 3つのレンダーターゲットハンドルを入れる配列を用意
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[3];

    // SV_TARGET0 用 (メインカラー)

    rtvHandles[0] = renderTextureDataNormal.rtvHandleCPU;
    // SV_TARGET1 用 (温度バッファ) 
    rtvHandles[1] = renderTextureDataThermography.rtvHandleCPU;
    //SV_TARGET2 用 (IDバッファ)
    rtvHandles[2] = renderTextureDataID.rtvHandleCPU;

    //TransitionBarrierの設定
    barrierMain_.SettingBarrierSRVforRTV(renderTextureDataNormal.resource.Get());
    barrierMain_.SettingBarrierSRVforRTV(renderTextureDataThermography.resource.Get());
    barrierMain_.SettingBarrierSRVforRTV(renderTextureDataID.resource.Get());

    //描画用のRTVとDSVを設定する 
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvDescriptorHeap->GetDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
    // 第1引数を 3 に変更して3つのRTVを同時にバインド
    commandList->OMSetRenderTargets(3, rtvHandles, false, &dsvHandle);

    //指定した色で画面全体をクリアする
    Vector4 color = renderTexture_->GetColor();
    float clearColor[] = { color.x,color.y,color.z,color.w };//青っぽい色。RGBAの順
    commandList->ClearRenderTargetView(renderTextureDataNormal.rtvHandleCPU, clearColor, 0, nullptr);

    //指定した深度で画面全体をクリアする
    commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    //温度バッファ(Index 1)のクリア処理を追加
    D3D12_CPU_DESCRIPTOR_HANDLE rtvTemp = renderTexture_->GetRenderTextureData(RenderTexture::kThermography).rtvHandleCPU;
    commandList->ClearRenderTargetView(rtvTemp, clearColor, 0, nullptr);

    // IDバッファ(Index 2)のクリア処理
    commandList->ClearRenderTargetView(renderTextureDataID.rtvHandleCPU, clearColor, 0, nullptr);
}


void DirectXCommon::DrawRenderTexture(SwapChain& swapChainClass,RtvDescriptorHeap* rtvDescriptorHeap)
{
    auto* ppm = PostProcessManager::GetInstance();

    PostProcessManager::Layer modelLayer = PostProcessManager::kModel;
    //Executeまでにこれをセットしておく
    ppm->SetRenderTexture(renderTexture_.get(), modelLayer);
    ppm->ClearEffects(modelLayer);

    ppm->AddEffect(PSO::kEffectGrayScale, modelLayer);
    ppm->AddEffect(PSO::kEffectDepthBasedOutline, modelLayer);
    ppm->AddEffect(PSO::kEffectLuminanceBasedOutline, modelLayer);
    ppm->AddEffect(PSO::kEffectBoxFilter, modelLayer);
    ppm->AddEffect(PSO::kEffectGaussianFilter, modelLayer);
    ppm->AddEffect(PSO::kEffectRadialBlur, modelLayer);
    ppm->AddEffect(PSO::kEffectMosaic, modelLayer);
    ppm->AddEffect(PSO::kEffectVignette, modelLayer);
    //ppm.AddEffect(PSO::kEffectRandom);
    ppm->AddEffect(PSO::kEffectThermography, modelLayer);
    ppm->AddEffect(PSO::kEffectDissolve, modelLayer);

    PostProcessManager::Layer spriteLayer = PostProcessManager::kSprite;
    ppm->SetRenderTexture(renderTextureForSprite_.get(), spriteLayer);
    ppm->ClearEffects(spriteLayer);
    ppm->AddEffect(PSO::kEffectThermography, spriteLayer);
    ppm->AddEffect(PSO::kEffectDissolve, spriteLayer);

    //描画先を画面(バックバッファ)のRTVにする
    // バックバッファは PreDraw で既に RENDER_TARGET 状態になっています
    UINT backBufferIndex = swapChainClass.GetSwapChain()->GetCurrentBackBufferIndex();

    auto backBufferRTV = rtvDescriptorHeap->GetCPUDescriptorHandle(backBufferIndex);

    ppm->Execute(modelLayer, backBufferRTV, &barrierMain_, kBlendModeMultiply);


}

void DirectXCommon::DrawRenderTextureForSprite(SwapChain& swapChainClass,RtvDescriptorHeap* rtvDescriptorHeap)
{   
    auto* ppm = PostProcessManager::GetInstance();
    //描画先を画面(バックバッファ)のRTVにする
// バックバッファは PreDraw で既に RENDER_TARGET 状態になっています
    UINT backBufferIndex = swapChainClass.GetSwapChain()->GetCurrentBackBufferIndex();

    auto backBufferRTV = rtvDescriptorHeap->GetCPUDescriptorHandle(backBufferIndex);
    PostProcessManager::Layer spriteLayer = PostProcessManager::kSprite;
    // スプライトのエフェクトをバックバッファに「上書き（アルファブレンド）」出力
    ppm->Execute(spriteLayer, backBufferRTV, &barrierMain_, kBlendModeNormal);

}

void DirectXCommon::RenderTexturePostDraw(DepthTexture* depthTexture)
{

    if (Input::IsTriggerMouse(0)) {
        Vector2Int pos = Input::GetCursorPositionInt();
        renderTexture_->CopyClickPixelCommand(pos.x, pos.y);
    }

    barrierMain_.SettingBarrier(
        depthTexture->GetResource(),
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    auto& renderTextureData = renderTexture_->GetRenderTextureData(RenderTexture::kNormal0);
    barrierMain_.SettingBarrierRTVforSRV(renderTextureData.resource.Get());

    auto& renderTextureDataThermography = renderTexture_->GetRenderTextureData(RenderTexture::kThermography);
    barrierMain_.SettingBarrierRTVforSRV(renderTextureDataThermography.resource.Get());

    // ID用テクスチャのバリアを元に戻す
    auto& renderTextureDataID = renderTexture_->GetRenderTextureData(RenderTexture::kObjectID);
    barrierMain_.SettingBarrierRTVforSRV(renderTextureDataID.resource.Get());
}



void DirectXCommon::SettingIdTextureBarrierPost()
{

    auto& renderTextureDataID = renderTexture_->GetRenderTextureData(RenderTexture::kObjectID);

    barrierMain_.SettingBarrier(
        renderTextureDataID.resource.Get(),
        D3D12_RESOURCE_STATE_COPY_SOURCE,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
}

void DirectXCommon::SettingIdTextureBarrierPre()
{
    auto& renderTextureDataID = renderTexture_->GetRenderTextureData(RenderTexture::kObjectID);
    barrierMain_.SettingBarrier(
        renderTextureDataID.resource.Get(),
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
        D3D12_RESOURCE_STATE_COPY_SOURCE);
}

void DirectXCommon::PreDraw(SwapChain& swapChainClass,CommandListManager& commandListManager)
{
    //これから書き込むバックバッファのインデックスを取得
    UINT backBufferIndex = swapChainClass.GetSwapChain()->GetCurrentBackBufferIndex();

    auto* commanList = commandListManager.GetCommandList(CommandListManager::kMain);

    //TransitionBarrierの設定
    barrierMain_.SettingBarrier(
        swapChainClass.GetCurrentResource(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    commanList->OMSetRenderTargets(1, &rtvClass.GetHandle(backBufferIndex), false, nullptr);
    //指定した色で画面全体をクリアする
    Vector4 color = renderTexture_->GetColor();
    float clearColor[] = { color.x,color.y,color.z,color.w };
    commanList->ClearRenderTargetView(rtvClass.GetHandle(backBufferIndex), clearColor, 0, nullptr);

}

void DirectXCommon::RenderTextureForSpritePreDraw(CommandListManager& commandListManager)
{

    auto* commanList = commandListManager.GetCommandList(CommandListManager::kMain);

    auto& renderTextureDataNormal = renderTextureForSprite_->GetRenderTextureData(RenderTexture::kNormal0);
    auto& renderTextureDataThermography = renderTextureForSprite_->GetRenderTextureData(RenderTexture::kThermography);

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
    rtvHandles[0] = renderTextureDataNormal.rtvHandleCPU;
    rtvHandles[1] = renderTextureDataThermography.rtvHandleCPU;

    // SRVからRTVへバリア遷移
    barrierMain_.SettingBarrierSRVforRTV(renderTextureDataNormal.resource.Get());
    barrierMain_.SettingBarrierSRVforRTV(renderTextureDataThermography.resource.Get());

    // スプライト描画用のRTVをセット (深度バッファは不要なら nullptr)
    commanList->OMSetRenderTargets(2, rtvHandles, false, nullptr);

    //指定した色で画面全体をクリアする
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    commanList->ClearRenderTargetView(rtvHandles[0], clearColor, 0, nullptr);
    commanList->ClearRenderTargetView(rtvHandles[1], clearColor, 0, nullptr);

}

void DirectXCommon::RenderTextureForSpritePostDraw()
{// RTVからSRVへバリア遷移（これでPostProcessManagerがテクスチャとして読み込めるようになる）
    auto& renderTextureDataNormal = renderTextureForSprite_->GetRenderTextureData(RenderTexture::kNormal0);
    auto& renderTextureDataThermography = renderTextureForSprite_->GetRenderTextureData(RenderTexture::kThermography);

    barrierMain_.SettingBarrierRTVforSRV(renderTextureDataNormal.resource.Get());
    barrierMain_.SettingBarrierRTVforSRV(renderTextureDataThermography.resource.Get());
}

// =============================================================================================
void DirectXCommon::InitializeDevice()
{
#ifdef _DEBUG
    ComPtr <ID3D12Debug1> debugController = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        //デバッグレイヤーを有効化する
        debugController->EnableDebugLayer();
        //さらにGPU側でもチェックを行うようにする
        debugController->SetEnableGPUBasedValidation(TRUE);
    }
#endif

    //DXGIFactoryの生成
    dxgiFactory.Create();
    LogFile::Log("CreateDXGIFactory\n");

    //使用するアダプタ(GPU)を決定する
    gpu.SettingGPU(dxgiFactory);
    LogFile::Log("Set GPU\n");

    //D3D12Deviceの生成
    device = CreateD3D12Device(gpu);
    //ファイルへのログ出力
    LogFile::Log("Complete create D3D12Device!!!\n");//初期化完了のログを出す

#ifdef _DEBUG
    debugError.Create(device);
    LogFile::Log("SetDebugError\n");
#endif
    LogFile::Log("DirectXCommon PreInitialize");
}


void DirectXCommon::InitializeRenderTargetView(SwapChain& swapChainClass,RtvDescriptorHeap* rtvDescriptorHeap)
{
    //RTVを作る
    rtvClass.Create(swapChainClass.GetResources(), rtvDescriptorHeap);
    LogFile::Log("CreateRTV\n");
}

void DirectXCommon::UpdateGameScreen(CbvSrvUavDescriptorHeap* srvDescriptorHeap)
{    //一旦ここでゲーム画面の描画
    DebugUI::ShowMainViewPort(srvDescriptorHeap, renderTexture_->GetRenderTextureData(RenderTexture::kNormal1).resource.srvIndex);
}


void DirectXCommon::CreateDXCCompiler()
{
    dxcCompiler = std::make_unique<DxcCompiler>();
    dxcCompiler->Initialize();
    dxcCompiler->ShaderSetting();
    LogFile::Log("Init DxcCompiler\n");
}

void DirectXCommon::InitializeRenderTexture(CommandListManager& commandListManager,RtvDescriptorHeap* rtvDescriptorHeap, CbvSrvUavDescriptorHeap* srvDescriptorHeap)
{
    renderTexture_ = std::make_unique<RenderTexture>();
    auto* commanList = commandListManager.GetCommandList(CommandListManager::kMain);

    renderTexture_->SetCommandListAndSrvDescriptorHeap(commanList, srvDescriptorHeap);
    //グレー
    renderTexture_->Create(rtvDescriptorHeap, { 0.5f,0.5f,0.5f,1.0f });

    //スプライト用のレンダーテクスチャを作成
    renderTextureForSprite_ = std::make_unique<RenderTexture>();
    renderTextureForSprite_->SetCommandListAndSrvDescriptorHeap(commanList, srvDescriptorHeap);
    renderTextureForSprite_->Create(rtvDescriptorHeap, { 0.0f,0.0f,0.0f,0.0f });

}

void DirectXCommon::UpdateRenderTexture(CbvSrvUavDescriptorHeap* srvDescriptorHeap)
{

#ifdef USE_IMGUI
    ImGui::Begin("PostEffect Viewer");

    // 例：表示したいSRVのインデックス番号
    for (auto& textureData : renderTexture_->GetRenderTextureDatas()) {
        DebugUI::CheckSRVTexture(textureData.resource.srvIndex, srvDescriptorHeap);
    }
    for (auto& textureData : renderTextureForSprite_->GetRenderTextureDatas()) {
        DebugUI::CheckSRVTexture(textureData.resource.srvIndex, srvDescriptorHeap);
    }
    ImGui::End();

#endif
}

// =============================================================================================

ComPtr<ID3D12DescriptorHeap> DirectXCommon::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
    //ディスクリプタヒープを生成する
    ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
    descriptorHeapDesc.Type = heapType;//レンダーターゲットビュー用
    descriptorHeapDesc.NumDescriptors = numDescriptors;
    descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT hr = device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
    //ディスクリプタヒープが作れなかったので起動できない
    assert(SUCCEEDED(hr));
    return descriptorHeap;
}
