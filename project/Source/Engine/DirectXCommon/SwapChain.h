#pragma once
#include<dxgi1_6.h>
#include<d3d12.h>
//ComPtr(コムポインタ)
#include<wrl.h>
#include"Window.h"
#include"ResourceManager/ResourceManager.h"
#include<array>

class SwapChain
{
public:
    SwapChain(Window& window,
        const Microsoft::WRL::ComPtr<IDXGIFactory7>& dxgiFactory,
        const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& commandQueue);

    Microsoft::WRL::ComPtr <IDXGISwapChain4>& GetSwapChain() {
        return swapChain_;
    };
    DXGI_SWAP_CHAIN_DESC1& GetDesc() {
        return swapChainDesc_
            ;
    };
    void GetBuffer(UINT buffer, Microsoft::WRL::ComPtr <ID3D12Resource>& resource);

    const Microsoft::WRL::ComPtr <ID3D12Resource>& GetCurrentResource();
    ~SwapChain();
    std::array<IResource, 2>& GetResources() { return resources_; };
private:
    Microsoft::WRL::ComPtr <IDXGISwapChain4> swapChain_ = nullptr;
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{};
    std::array<IResource, 2> resources_{};
};

