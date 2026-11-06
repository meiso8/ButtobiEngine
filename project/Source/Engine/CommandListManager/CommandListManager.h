#pragma once

#include <memory>
#include<unordered_map>
#include<d3d12.h>
#include<wrl.h>
#include"Fence.h"

#include"CommandQueue.h"

class CommandList;

class TransitionBarrier;
class Window;
class SwapChain;
class CommandListManager

{
public:
    enum Type {
        kLoad,
        kMain,
    };
private:
    std::unordered_map<Type, std::unique_ptr<CommandList>>commandList_{};
    std::unique_ptr <CommandQueue> commandQueue_ = nullptr;
    std::unique_ptr<Fence>fence_ = nullptr;

    D3D12_VIEWPORT viewport_ = {};
    D3D12_RECT scissorRect_ = {};

    void InitializeCommandList();
    void InitializeCommandQueue(const Microsoft::WRL::ComPtr<ID3D12Device>& device);
    void InitializeFence(const Microsoft::WRL::ComPtr<ID3D12Device>& device);
    void InitializeViewport(const Window& window);
    void InitializeScissorRect(const Window& window);
public:
    CommandListManager(const Microsoft::WRL::ComPtr<ID3D12Device>& device, const Window& window);

    /// @brief コマンドリストの取得関数
    /// @return コマンドリスト
    ID3D12GraphicsCommandList* GetCommandList(const Type type);

    const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue();
    //    /// @brief コマンドリストクラスの取得関数
    ///// @return コマンドリスト
    //    CommandList* GetCommandListClass(const CommandListType type) { return commandList_[type].get(); };
    void SetViewPortAndScissorRect(const Type type = kMain);

    /// @brief 次フレームの準備
    void PrepareCommand(const Type type);
    //コマンドリストを閉じる
    void CloseLoadCommandList(const Type type);

    void PostDraw(TransitionBarrier& barrier, SwapChain& swapChainClass);
};

