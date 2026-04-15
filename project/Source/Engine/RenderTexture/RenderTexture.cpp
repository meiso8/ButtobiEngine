#include "RenderTexture.h"
#include"DirectXCommon.h"
#include"Engine/SRVManager/SRVManager.h"

void RenderTexture::Create()
{
    kRenderTargetClearValue_ = { 1.0f,0.0f,0.0f,1.0f };

    //rtvの作成
    renderTextureData_.resource =
        DirectXCommon::CreateRenderTextureResource(
            Window::GetClientWidth(),
            Window::GetClientHeight(),
            DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
            kRenderTargetClearValue_
        );


    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    renderTextureData_.rtvHandleCPU = DirectXCommon::GetRTVCPUDescriptorHandle(3);
    DirectXCommon::GetDevice()->CreateRenderTargetView(renderTextureData_.resource.Get(), &rtvDesc, renderTextureData_.rtvHandleCPU);


    //SRVの作成
    D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
    renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    renderTextureSrvDesc.Texture2D.MipLevels = 1;

    // ------------------------------------------
    renderTextureData_.srvIndex =  SrvManager::Allocate();
    renderTextureData_.srvHandleCPU = SrvManager::GetCPUDescriptorHandle(renderTextureData_.srvIndex);
    renderTextureData_.srvHandleGPU = SrvManager::GetGPUDescriptorHandle(renderTextureData_.srvIndex);

    DirectXCommon::GetDevice()->CreateShaderResourceView(renderTextureData_.resource.Get(), &renderTextureSrvDesc, renderTextureData_.srvHandleCPU);
}
