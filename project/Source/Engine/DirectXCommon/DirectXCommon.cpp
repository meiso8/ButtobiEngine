#include "DirectXCommon.h"
#include"Log.h"
#include "Viewport.h"
#include "ScissorRect.h"
#include<cassert>

#include"CbvSrvUavDescriptorHeap.h"
#include"RtvDescriptorHeap.h"
#include"DsvDescriptorHeap.h"

#include "PostProcessManager/PostProcessManager.h"

#include"DebugUI.h"
#include"Input.h"
#include"ResourceManager/ResourceManager.h"
#include"DepthTexture/DepthTexture.h"

using namespace Microsoft::WRL;
ComPtr<ID3D12Device> DirectXCommon::device = nullptr;
std::unique_ptr< DxcCompiler> DirectXCommon::dxcCompiler = nullptr;

DirectXCommon::~DirectXCommon()
{

    renderTexture_->Clear();
    renderTextureForSprite_->Clear();
  
    for (auto& resource : swapChainResources) {
        resource.Reset();
    }

    if (device) {
        device.Reset();
  }
  
}

void DirectXCommon::Finalize()
{
    LogFile::Log("DirectXCommon Finalize\n");
}

void DirectXCommon::PreInitialize(Window& window)
{
    assert(&window);

    window_ = &window;

    InitializeDevice();
    InitializeCommand();
    CreateSwapChain();
    //コマンドリストのセット
    barrierMain_.SetCommandList(commandList_[kMain]->Get());
    barrierLoad_.SetCommandList(commandList_[kLoad]->Get());

}
void DirectXCommon::PostInitialize()
{
    InitializeFence();
    InitializeViewPort();
    ScissorRectSetting();
    CreateDXCCompiler();

}

void DirectXCommon::RenderTexturePreDraw(DsvDescriptorHeap* dsvDescriptorHeap, DepthTexture* depthTexture)
{

    auto* commandList = commandList_[kMain]->Get();

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

void DirectXCommon::ViewPortAndScissorRect()

{
    auto* commanList = commandList_[kMain]->Get();
    //ビューポート領域の設定
    commanList->RSSetViewports(1, &viewport);//Viewportを設定
    //シザー矩形の設定
    commanList->RSSetScissorRects(1, &scissorRect);//Scissorを設定
}


void DirectXCommon::DrawRenderTexture(RtvDescriptorHeap* rtvDescriptorHeap)
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

void DirectXCommon::DrawRenderTextureForSprite(RtvDescriptorHeap* rtvDescriptorHeap)
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

void DirectXCommon::PreDraw()
{
    //これから書き込むバックバッファのインデックスを取得
    UINT backBufferIndex = swapChainClass.GetSwapChain()->GetCurrentBackBufferIndex();

    auto* commanList = commandList_[kMain]->Get();

    //TransitionBarrierの設定
    barrierMain_.SettingBarrier(
        swapChainResources[backBufferIndex],
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    commanList->OMSetRenderTargets(1, &rtvClass.GetHandle(backBufferIndex), false, nullptr);
    //指定した色で画面全体をクリアする
    Vector4 color = renderTexture_->GetColor();
    float clearColor[] = { color.x,color.y,color.z,color.w };
    commanList->ClearRenderTargetView(rtvClass.GetHandle(backBufferIndex), clearColor, 0, nullptr);

}

void DirectXCommon::PostDraw()
{
    //これから書き込むバックバッファのインデックスを取得
    UINT backBufferIndex = swapChainClass.GetSwapChain()->GetCurrentBackBufferIndex();

    auto* commanList = commandList_[kMain]->Get();

    //TransitionBarrierの設定
    barrierMain_.SettingBarrier(
        swapChainResources[backBufferIndex],
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);

    //コマンドリストの内容を確定させる。全てのコマンドを積んでから　Closesする。
    commanList->Close();
    //GPUにコマンドリストの実行を行わせる
    
    ID3D12CommandList* commandLists[] = { commanList };
    commandQueue.GetCommandQueue()->ExecuteCommandLists(1, commandLists);

    //GPUとOSに画面の交換を行うよう通知する
    swapChainClass.GetSwapChain()->Present(1, 0);

    //画面の更新が終わった直後GPUにシグナルを送る
    fence.SendSignal(commandQueue);

}

void DirectXCommon::PrepareCommand(const CommandListType type)
{    //次のフレーム用のコマンドリストを準備
    commandList_[type]->PrepareCommand();
}

void DirectXCommon::CloseLoadCommandList(const CommandListType type)
{
    auto* commanList = commandList_[type]->Get();

    //コマンドリストの内容を確定させる。全てのコマンドを積んでから　Closesする。
    commanList->Close();

    //GPUにコマンドリストの実行を行わせる
    ID3D12CommandList* commandLists[] = { commanList };
    commandQueue.GetCommandQueue()->ExecuteCommandLists(1, commandLists);

    //画面の更新が終わった直後GPUにシグナルを送る
    fence.SendSignal(commandQueue);
}

void DirectXCommon::RenderTextureForSpritePreDraw()
{
    auto* commanList = commandList_[kMain]->Get();

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

}

void DirectXCommon::InitializeCommand()
{
    commandList_[kMain] = std::make_unique<CommandList>();
    commandList_[kLoad] = std::make_unique<CommandList>();

    for (auto& [type,commandList] : commandList_) {
        //コマンドリストの生成
        commandList->Create();
    }

    LogFile::Log("Create CommandList\n");

    //コマンドキューの生成
    commandQueue.Create(device);
    LogFile::Log("Create CommandQueue\n");

}

void DirectXCommon::CreateSwapChain()
{

    //スワップチェインの生成
//画面の幅。ウィンドウのクライアント領域を同じものにしておく
    swapChainClass.Create(
        *window_,
        dxgiFactory.GetDxgiFactory(),
        commandQueue.GetCommandQueue());
    LogFile::Log("CreateSwapChain\n");

#pragma region//SwapChainからResourceを引っ張ってくる
    //SwapChainからResourceを引っ張ってくる
    swapChainClass.GetBuffer(0, swapChainResources[0]);
    swapChainClass.GetBuffer(1, swapChainResources[1]);
    LogFile::Log("Pull Resource from SwapChain\n");

#pragma endregion

}

void DirectXCommon::InitializeRenderTargetView(RtvDescriptorHeap* rtvDescriptorHeap)
{
    //RTVを作る
    rtvClass.Create(swapChainResources, rtvDescriptorHeap);
    LogFile::Log("CreateRTV\n");
}

void DirectXCommon::UpdateGameScreen(CbvSrvUavDescriptorHeap* srvDescriptorHeap)
{    //一旦ここでゲーム画面の描画
    DebugUI::ShowMainViewPort(srvDescriptorHeap, renderTexture_->GetRenderTextureData(RenderTexture::kNormal1).resource.srvIndex);
}

void DirectXCommon::InitializeFence()
{
#pragma region //FenceとEventを生成する
    fence.Create(device);
    LogFile::Log("CreateFence\n");
#pragma endregion
}

void DirectXCommon::InitializeViewPort()
{
    viewport = CreateViewport(static_cast<float>(window_->GetClientWidth()), static_cast<float>(window_->GetClientHeight()));

    LogFile::Log("InitializeViewPort\n");
}

void DirectXCommon::ScissorRectSetting()
{
    //ViewportとScissor(シザー)
    scissorRect = CreateScissorRect(window_->GetClientWidth(), window_->GetClientHeight());
    LogFile::Log("Viewport And Scissor\n");
}

void DirectXCommon::CreateDXCCompiler()
{
    dxcCompiler = std::make_unique<DxcCompiler>();
    dxcCompiler->Initialize();
    dxcCompiler->ShaderSetting();
    LogFile::Log("Init DxcCompiler\n");
}

void DirectXCommon::InitializeRenderTexture(RtvDescriptorHeap* rtvDescriptorHeap, CbvSrvUavDescriptorHeap* srvDescriptorHeap)
{
    renderTexture_ = std::make_unique<RenderTexture>();
    renderTexture_->SetCommandListAndSrvDescriptorHeap(commandList_[kMain]->Get(), srvDescriptorHeap);
    //グレー
    renderTexture_->Create(rtvDescriptorHeap, { 0.5f,0.5f,0.5f,1.0f });

    //スプライト用のレンダーテクスチャを作成
    renderTextureForSprite_ = std::make_unique<RenderTexture>();
    renderTextureForSprite_->SetCommandListAndSrvDescriptorHeap(commandList_[kMain]->Get(), srvDescriptorHeap);
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
