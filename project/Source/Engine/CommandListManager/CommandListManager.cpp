#include "CommandListManager.h"
#include"Log.h"

#include"CommandList.h"


#include "Viewport.h"
#include "ScissorRect.h"
#include"Window.h"
#include"SwapChain.h"
#include"TransitionBarrier.h"
#include"Device/Device.h"


ID3D12GraphicsCommandList* CommandListManager::GetCommandList(const Type type)
{
    return commandList_[type]->GetCommandList().Get();
}

const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& CommandListManager::GetCommandQueue()
{
    return commandQueue_->GetCommandQueue();
}

void CommandListManager::SetViewPortAndScissorRect(const Type type)
{
    auto* commanList = commandList_[type]->Get();
    //ビューポート領域の設定
    commanList->RSSetViewports(1, &viewport_);//Viewportを設定
    //シザー矩形の設定
    commanList->RSSetScissorRects(1, &scissorRect_);//Scissorを設定
}
void CommandListManager::PostDraw(TransitionBarrier& barrier, SwapChain& swapChainClass)
{
    auto* commanList = commandList_[kMain]->Get();

    //TransitionBarrierの設定
    barrier.SettingBarrier(
        swapChainClass.GetCurrentResource(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);

    //コマンドリストの内容を確定させる。全てのコマンドを積んでから　Closesする。
    commanList->Close();
    //GPUにコマンドリストの実行を行わせる

    ID3D12CommandList* commandLists[] = { commanList };
    commandQueue_->GetCommandQueue()->ExecuteCommandLists(1, commandLists);

    //GPUとOSに画面の交換を行うよう通知する
    swapChainClass.GetSwapChain()->Present(1, 0);

    //画面の更新が終わった直後GPUにシグナルを送る
    fence_->SendSignal(*commandQueue_);
}

void CommandListManager::PrepareCommand(const Type type)
{
    //次のフレーム用のコマンドリストを準備
    commandList_[type]->PrepareCommand();
}
void CommandListManager::CloseLoadCommandList(const Type type)
{
    auto* commanList = commandList_[type]->Get();

    //コマンドリストの内容を確定させる。全てのコマンドを積んでから　Closesする。
    commanList->Close();

    //GPUにコマンドリストの実行を行わせる
    ID3D12CommandList* commandLists[] = { commanList };
    commandQueue_->GetCommandQueue()->ExecuteCommandLists(1, commandLists);

    //画面の更新が終わった直後GPUにシグナルを送る
    fence_->SendSignal(*commandQueue_);
}


CommandListManager::CommandListManager(Device& device, const Window& window)
{
    InitializeCommandList(device);
    InitializeCommandQueue(device);
    InitializeFence(device);
    InitializeViewport(window);
    InitializeScissorRect(window);
}

void CommandListManager::InitializeFence(Device& deviceClass)
{
#pragma region //FenceとEventを生成する
    fence_ = std::make_unique<Fence>(deviceClass);
    LogFile::Log("CreateFence\n");
#pragma endregion
}

void CommandListManager::InitializeCommandList(Device& deviceClass)
{
    commandList_[kMain] = std::make_unique<CommandList>(deviceClass);
    commandList_[kLoad] = std::make_unique<CommandList>(deviceClass);
    LogFile::Log("Create CommandList\n");
}

void CommandListManager::InitializeCommandQueue(Device& deviceClass)
{
    //コマンドキューの生成
    commandQueue_ = std::make_unique<CommandQueue>(deviceClass);
    LogFile::Log("Create CommandQueue\n");
}

void CommandListManager::InitializeViewport(const Window& window)
{
    viewport_ = CreateViewport(static_cast<float>(window.GetClientWidth()), static_cast<float>(window.GetClientHeight()));
    LogFile::Log("InitializeViewPort\n");
}

void CommandListManager::InitializeScissorRect(const Window& window)
{    //ViewportとScissor(シザー)
    scissorRect_ = CreateScissorRect(window.GetClientWidth(), window.GetClientHeight());
    LogFile::Log("Viewport And Scissor\n");
}

