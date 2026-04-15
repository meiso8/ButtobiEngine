#include "TransitionBarrier.h"

//TransitionBarrierの設定
void TransitionBarrier::SettingBarrier(
    const Microsoft::WRL::ComPtr <ID3D12Resource>& swapChainResources,
    D3D12_RESOURCE_STATES stateBefore,
    D3D12_RESOURCE_STATES stateAfter,
    ID3D12GraphicsCommandList* commandList) {

    //今回はバリアはTransition
    barrier_.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    //Noneにしておく
    barrier_.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    //バリアを張る対象のリソース。現在のバックバッファに対して行う
    barrier_.Transition.pResource = swapChainResources.Get();
    //遷移前（現在）のResourceState
    barrier_.Transition.StateBefore = stateBefore;
    //遷移後のResourceState
    barrier_.Transition.StateAfter = stateAfter;
    //TransitionBarrierを張る
    commandList->ResourceBarrier(1, &barrier_);
}


