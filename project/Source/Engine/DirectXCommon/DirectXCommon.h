#pragma once

#include"DXGIFactory.h"
#include"GPU.h"
#include"SwapChain.h"
#include"DebugError.h"

#include"RenderTargetView.h"

#include"PSO/Depth.h"//StencilTextureの作成関数　奥行き
#include"CompileShader/CompileShader.h"

#include"Window.h"
#include<array>
#include"ImGuiClass.h"

#include <memory>

#include"PostEffectMaterial/PostEffectMaterial.h"
//Textureデータを読み込むためにDirectXTex.hをインクルード
#include"../externals/DirectXTex/DirectXTex.h"

#include"Vector4.h"
#include"../RenderTexture/RenderTexture.h"
#include"TransitionBarrier.h"
#include"Device/Device.h"

struct TextureResource;
class DepthTexture;
class CbvSrvUavDescriptorHeap;
class RtvDescriptorHeap;
class DsvDescriptorHeap;
class CommandListManager;

class DirectXCommon
{


private:
    std::unique_ptr<Device>device_ = nullptr;
    DXGIFactory dxgiFactory_ = {};

    GPU gpu_ = {};
    DebugError debugError_ = {};

    RenderTargetView rtvClass = {};
    std::unique_ptr<RenderTexture> renderTexture_ = nullptr;
    std::unique_ptr<RenderTexture> renderTextureForSprite_ = nullptr;
    TransitionBarrier barrierMain_;
    TransitionBarrier barrierLoad_;
public:
    RenderTexture* GetRenderTexture() { return renderTexture_.get(); };
    void Finalize();
    ~DirectXCommon();
    
    /// @brief 初期化
    /// @param window windowクラスを渡す
    void InitializeDevice();
    void InitializeBarrier(CommandListManager& commandListManager);

    /// @brief 描画前処理
    /// @param color 画面の色を指定する
    void RenderTexturePreDraw(CommandListManager& commandListManager, DsvDescriptorHeap* dsvDescriptorHeap, DepthTexture* depthTexture);
    void DrawRenderTexture(SwapChain& swapChainClass,RtvDescriptorHeap* rtvDescriptorHeap);
    void DrawRenderTextureForSprite(SwapChain& swapChainClass,RtvDescriptorHeap* rtvDescriptorHeap);
    void RenderTexturePostDraw(DepthTexture* depthTexture);

    void SettingIdTextureBarrierPre();
    void SettingIdTextureBarrierPost();

    /// @brief 描画前処理
    void PreDraw(SwapChain& swapChainClass,CommandListManager& commandListManager);

    void RenderTextureForSpritePreDraw(CommandListManager& commandListManager);
    void RenderTextureForSpritePostDraw();
    /// @brief スワップチェインのRTVの取得関数
    /// @return スワップチェインのRTV
    RenderTargetView& GetSwapChainRtv() {
        return rtvClass ;
    }
    
    void InitializeRenderTexture(CommandListManager& commandListManager,RtvDescriptorHeap* rtvDescriptorHeap, CbvSrvUavDescriptorHeap* srvDescriptorHeap);
    void InitializeRenderTargetView(SwapChain& swapChainClass,RtvDescriptorHeap* rtvDescriptorHeap);

    void UpdateGameScreen(CbvSrvUavDescriptorHeap* srvDescriptorHeap);
    void UpdateRenderTexture(CbvSrvUavDescriptorHeap* srvDescriptorHeap);
    DXGIFactory& GetDXGIFactory() { return dxgiFactory_; };
    TransitionBarrier& GetMainBarrier() { return barrierMain_; };
    Device* GetDeviceClass() { return device_.get(); }
};

