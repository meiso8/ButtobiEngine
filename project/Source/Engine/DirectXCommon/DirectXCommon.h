#pragma once

#include"DXGIFactory.h"
#include"GPU.h"
#include"SwapChain.h"
#include"DebugError.h"

#include"RenderTargetView.h"

#include"PSO/Depth.h"//StencilTextureの作成関数　奥行き
#include"PSO/CompileShader.h"

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
struct TextureResource;
class DepthTexture;
class CbvSrvUavDescriptorHeap;
class RtvDescriptorHeap;
class DsvDescriptorHeap;
class CommandListManager;

class DirectXCommon
{


private:
    DXGIFactory dxgiFactory = {};

    static Microsoft::WRL::ComPtr<ID3D12Device> device;
    static std::unique_ptr< DxcCompiler> dxcCompiler;


    GPU gpu = {};
    DebugError debugError = {};

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
    void PostInitialize();

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

    /// @brief DescriptorHeapの作成関数
    /// @param heapType ヒープタイプの指定
    /// @param numDescriptors 
    /// @param shaderVisible 
    /// @return  DescriptorHeap
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
        D3D12_DESCRIPTOR_HEAP_TYPE heapType,
        UINT numDescriptors,
        bool shaderVisible);


    /// @brief デバイスの取得関数
    /// @return  デバイス
    static Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice() { return device; };
    /// @brief コンパイラーの取得関数
    /// @return コンパイラー
    static DxcCompiler* GetDxcCompiler() { return dxcCompiler.get(); }
     DXGIFactory& GetDXGIFactory() { return dxgiFactory; };
      TransitionBarrier& GetMainBarrier() { return barrierMain_; };
private:

    void CreateDXCCompiler();
};

