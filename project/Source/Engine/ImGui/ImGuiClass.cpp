#include "ImGuiClass.h"
#include"CommandList.h"
#include"SRVmanager/SrvManager.h"


void ImGuiClass::Initialize(Window& window,
    const Microsoft::WRL::ComPtr<ID3D12Device>& device,
    SwapChain& swapChain,
    RenderTargetView& rtv) {
    
    uint32_t srvIndex = SrvManager::Allocate();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplWin32_Init(window.GetHwnd());
    ImGui_ImplDX12_Init(device.Get(),
        swapChain.GetDesc().BufferCount,
        rtv.GetDesc().Format,
        SrvManager::GetDescriptorHeap(),
        SrvManager::GetCPUDescriptorHandle(srvIndex),
        SrvManager::GetGPUDescriptorHandle(srvIndex));
}

void ImGuiClass::FrameStart() {


    ImGui_ImplDX12_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

}

void ImGuiClass::Render() {

    //ImGuiの内部コマンドを生成する
    ImGui::Render();

}

void ImGuiClass::DrawImGui(ID3D12GraphicsCommandList* commandList) {

    //諸々の描画処理が終了下タイミングでImGuiの描画コマンドを積む
//実際のcommandListのImGuiの描画コマンドを積む
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

void ImGuiClass::ShutDown() {

    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

