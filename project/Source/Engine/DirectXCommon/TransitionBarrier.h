#pragma once
#include<wrl.h>
#include<d3d12.h>

class TransitionBarrier
{
public:
    void SettingBarrier(
        const Microsoft::WRL::ComPtr <ID3D12Resource>& swapChainResources,
        D3D12_RESOURCE_STATES stateBefore,
        D3D12_RESOURCE_STATES stateAfter,
        ID3D12GraphicsCommandList* commandList);
private:
    D3D12_RESOURCE_BARRIER barrier_{};
};

