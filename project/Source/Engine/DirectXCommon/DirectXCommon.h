#pragma once

#include"CommandQueue.h"
#include"commandList.h"
#include"DXGIFactory.h"
#include"GPU.h"
#include"SwapChain.h"
#include"DebugError.h"

#include"RenderTargetView.h"
#include"Fence.h"
#include"PSO/Depth.h"//StencilTextureの作成関数　奥行き
#include"PSO/CompileShader.h"

#include"Window.h"
#include<array>
#include"ImGuiClass.h"
#include"TransitionBarrier.h"

//Textureデータを読み込むためにDirectXTex.hをインクルード
#include"../externals/DirectXTex/DirectXTex.h"
//Textureの転送のために
#include"../externals/DirectXTex/d3dx12.h"
#include<chrono>

#include"Vector4.h"

class DirectXCommon
{
public:
    static uint32_t descriptorSizeRTV;
    static uint32_t descriptorSizeDSV;

    static const uint32_t kMaxSoundCount;
    static const uint32_t kMaxModelCount;
private:
    Window* window_ = nullptr;
    DXGIFactory dxgiFactory = {};

    static Microsoft::WRL::ComPtr<ID3D12Device> device;
    static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvDescriptorHeap;

    static Microsoft::WRL::ComPtr <ID3D12DescriptorHeap> dsvDescriptorHeap;

    static std::unique_ptr< DxcCompiler> dxcCompiler;
    static std::unique_ptr<CommandList> commandList;

    CommandQueue commandQueue = {};
    SwapChain swapChainClass;
    GPU gpu = {};
    DebugError debugError = {};
    std::array<Microsoft::WRL::ComPtr <ID3D12Resource>, 2> swapChainResources;
    RenderTargetView rtvClass = {};
    Fence fence = {};
    Microsoft::WRL::ComPtr <ID3D12Resource> depthStencilResource = nullptr;

    D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
    D3D12_VIEWPORT viewport = {};
    D3D12_RECT scissorRect = {};
    TransitionBarrier barrier = {};
    std::chrono::steady_clock::time_point reference_;


public:

    ~DirectXCommon();

    void Initialize(Window& window);
    void PreDraw(Vector4& color);
    void PostDraw();
    void EndFrame();

    SwapChain& GetSwapChain() { return swapChainClass; };
    RenderTargetView& GetSwapChainRtv() {
        return rtvClass
            ;
    }
    static Microsoft::WRL::ComPtr<ID3D12Resource> CreateBufferResource(
        size_t sizeInBytes);

   static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(
        D3D12_DESCRIPTOR_HEAP_TYPE heapType,
        UINT numDescriptors,
        bool shaderVisible);

    static Microsoft::WRL::ComPtr<ID3D12Resource> CreateTextureResource(
        const DirectX::TexMetadata& metadata);


    [[nodiscard]]
    static Microsoft::WRL::ComPtr<ID3D12Resource> UploadTextureData(
        const Microsoft::WRL::ComPtr<ID3D12Resource>& texture,
        const DirectX::ScratchImage& mipImages);
    static Microsoft::WRL::ComPtr<ID3D12Device> GetDevice() { return device; };
    static DxcCompiler* GetDxcCompiler() { return dxcCompiler.get(); }
    static ID3D12GraphicsCommandList* GetCommandList() { return commandList->GetCommandList().Get(); };

  static D3D12_CPU_DESCRIPTOR_HANDLE GetRTVCPUDescriptorHandle(uint32_t index);
    static D3D12_GPU_DESCRIPTOR_HANDLE GetRTVGPUDescriptorHandle(uint32_t index);
    static D3D12_CPU_DESCRIPTOR_HANDLE GetDSVCPUDescriptorHandle(uint32_t index);
    static D3D12_GPU_DESCRIPTOR_HANDLE GetDSVGPUDescriptorHandle(uint32_t index);
  
    
    
    static D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);
    static D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);


private:
 void InitializeDevice();
    void InitializeCommand();
    void CreateSwapChain();
    void CreateDepthBuffer();
    void DescriptorHeapSettings();
    void InitializeRenderTargetView();
    void InitializeDepthStencilView();
    void InitializeFence();
    void InitializeViewPort();
    void ScissorRectSetting();
    void CreateDXCCompiler();
    void InitializeFixFPS();
    void UpdateFixFPS();
};

