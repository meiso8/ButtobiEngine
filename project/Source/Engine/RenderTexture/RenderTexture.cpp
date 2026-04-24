#include "RenderTexture.h"
#include"DirectXCommon.h"
#include"Engine/SRVManager/SRVManager.h"
#include"PSO.h"
#ifdef _DEVELOP
#include "DebugUI.h"
#include"Log.h"

#endif
void RenderTexture::Create()
{
    kRenderTargetClearValue_ = { 1.0f,0.0f,0.0f,1.0f };

    CreateResource(0);
    CreateResource(1);

    CreateMaterialBUfferForFullScreen();
    CreateMaterialBufferForGrayScale();
    CreateMaterialBufferForVignette();
    CreateMaterialBufferForBoxFilter();

}

void RenderTexture::CreateResource(const uint32_t index)
{    //rtvの作成
    renderTextureDatas_[index].resource =
        DirectXCommon::CreateRenderTextureResource(
            Window::GetClientWidth(),
            Window::GetClientHeight(),
            DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
            kRenderTargetClearValue_
        );
    LogFile::Log("Rendertexture : CreateRTV");

    D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
    rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    renderTextureDatas_[index].rtvHandleCPU = DirectXCommon::GetRTVCPUDescriptorHandle(index+3);
    DirectXCommon::GetDevice()->CreateRenderTargetView(renderTextureDatas_[index].resource.Get(), &rtvDesc, renderTextureDatas_[index].rtvHandleCPU);

    LogFile::Log("Rendertexture : CreateRTVDesc");
    //SRVの作成
    D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
    renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
    renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    renderTextureSrvDesc.Texture2D.MipLevels = 1;
    LogFile::Log("Rendertexture : Create SRV");

    // ------------------------------------------
    renderTextureDatas_[index].srvIndex = SrvManager::Allocate();
    renderTextureDatas_[index].srvHandleCPU = SrvManager::GetCPUDescriptorHandle(renderTextureDatas_[index].srvIndex);
    renderTextureDatas_[index].srvHandleGPU = SrvManager::GetGPUDescriptorHandle(renderTextureDatas_[index].srvIndex);
    LogFile::Log("Rendertexture : GetSRVIndexAndGPUAndCPUHandle");

    DirectXCommon::GetDevice()->CreateShaderResourceView(renderTextureDatas_[index].resource.Get(), &renderTextureSrvDesc, renderTextureDatas_[index].srvHandleCPU);
    LogFile::Log("Rendertexture : CreateShaderResourceView");
}

void RenderTexture::Draw(const PSO::EffectType& effectType, const D3D12_CPU_DESCRIPTOR_HANDLE dstRtvHandle, const uint32_t index)
{
    auto* commandList = DirectXCommon::GetCommandList();
    // 1. 書き込み先（RTV）の設定とクリア
    commandList->OMSetRenderTargets(1, &dstRtvHandle, false, nullptr);
    commandList->SetGraphicsRootSignature(PSO::rootSignature->GetRootSignature(RootSignature::OFFSCREEN));
    commandList->SetPipelineState(PSO::GetGraphicsPipelineStateOffScreen(effectType).Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    //SRVのDescriptorTableの先頭を設定。0はrootParameter[0]である。
    SrvManager::SetGraphicsRootDescriptorTable(0, renderTextureDatas_[index].srvIndex);
    commandList->SetGraphicsRootConstantBufferView(1, materialResource_[effectType]->GetGPUVirtualAddress());
    commandList->DrawInstanced(3, 1, 0, 0);
}

void RenderTexture::Update()
{
#ifdef _DEVELOP
    if (ImGui::TreeNode("GrayScale")) {
        DebugUI::CheckColor(materialForGrayScale_->color, "RenderTextureColor");
        ImGui::Checkbox("UseEffect", &materialForGrayScale_->useEffect);
        const char* lights[] = { "Sepia", "Grayscale", "None" };
        int type_current = int(materialForGrayScale_->type);

        if (ImGui::Combo("EffectType", &type_current, lights, IM_ARRAYSIZE(lights))) {
            materialForGrayScale_->type = type_current % 3;
        };
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("Vignette")) {
        DebugUI::CheckFloat(materialForVignette_->correctVal,"correctVal");
        DebugUI::CheckFloat(materialForVignette_->viignetteVal, "viignetteVal");
        ImGui::TreePop();
    }

    if (ImGui::TreeNode("BoxFilter")) {
        ImGui::DragFloat( "kernel", &materialForBoxFilter_->kernel,1.0f,0.0f,1001.0f );
        ImGui::TreePop();
    }



#endif
}

void RenderTexture::CreateMaterialBufferForGrayScale()
{
    //マテリアル用のリソースを作る。
    materialResource_[PSO::kEffectGrayScale] = DirectXCommon::CreateBufferResource(sizeof(MaterialForRenderTexture));
    //マテリアルにデータを書き込む

    //書き込むためのアドレスを取得
    HRESULT result = materialResource_[PSO::kEffectGrayScale]->Map(0, nullptr, reinterpret_cast<void**>(&materialForGrayScale_));
    materialForGrayScale_->color = sepiaColor_;

    LogFile::Log("Rendertexture : Create : MaterialBuffer : GrayScale");
}

void RenderTexture::CreateMaterialBufferForVignette()
{    //マテリアル用のリソースを作る。
    materialResource_[PSO::kEffectVignette] = DirectXCommon::CreateBufferResource(sizeof(MaterialForVignette));
    //マテリアルにデータを書き込む

    //書き込むためのアドレスを取得
    HRESULT result = materialResource_[PSO::kEffectVignette]->Map(0, nullptr, reinterpret_cast<void**>(&materialForVignette_));
    materialForVignette_->correctVal = 16.0f;
    materialForVignette_->viignetteVal = 0.8f;

    LogFile::Log("Rendertexture : Create : MaterialBuffer : Vignette");
}

void RenderTexture::CreateMaterialBufferForBoxFilter()
{

    materialResource_[PSO::kEffectBoxFilter] = DirectXCommon::CreateBufferResource(sizeof(MaterialForBoxFilter));
    //マテリアルにデータを書き込む

    //書き込むためのアドレスを取得
    HRESULT result = materialResource_[PSO::kEffectBoxFilter]->Map(0, nullptr, reinterpret_cast<void**>(&materialForBoxFilter_));
    materialForBoxFilter_->kernel = 1.0f;

    LogFile::Log("Rendertexture : Create : MaterialBuffer : BoxFilter");
}

void RenderTexture::CreateMaterialBUfferForFullScreen()
{
    //マテリアル用のリソースを作る。
    materialResource_[PSO::kEffectNone] = DirectXCommon::CreateBufferResource(sizeof(MaterialForRenderTexture));
    //マテリアルにデータを書き込む

    //書き込むためのアドレスを取得
    HRESULT result = materialResource_[PSO::kEffectNone]->Map(0, nullptr, reinterpret_cast<void**>(&materialForFullScreen_));
    materialForFullScreen_->color = {1.0f,1.0f,1.0f,1.0f};

    LogFile::Log("Rendertexture : Create : MaterialBuffer : GrayScale");
}
