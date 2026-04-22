#include "RenderTexture.h"
#include"DirectXCommon.h"
#include"Engine/SRVManager/SRVManager.h"
#include"PSO.h"
#ifdef _DEVELOP
#include "DebugUI.h"
#endif
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

    CreateMaterialBuffer();
    CreateMaterialBufferForVignette();
    CreateMaterialBufferForBoxFilter();
}

void RenderTexture::Draw()
{
    auto* commandList = DirectXCommon::GetCommandList();

    commandList->SetGraphicsRootSignature(PSO::rootSignature->GetRootSignature(RootSignature::OFFSCREEN));
    commandList->SetPipelineState(PSO::GetGraphicsPipelineStateOffScreen().Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    ////SRVのDescriptorTableの先頭を設定。0はrootParameter[0]である。
    SrvManager::SetGraphicsRootDescriptorTable(0, renderTextureData_.srvIndex);
    commandList->SetGraphicsRootConstantBufferView(1, materialResource_->GetGPUVirtualAddress());
    //commandList->SetGraphicsRootConstantBufferView(1, materialResourceForVignette_->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(1, materialResourceForBoxFilter_->GetGPUVirtualAddress());
    commandList->DrawInstanced(3, 1, 0, 0);

}

void RenderTexture::Update()
{
#ifdef _DEVELOP
    if (ImGui::TreeNode("GrayScale")) {
        DebugUI::CheckColor(material_->color, "RenderTextureColor");
        ImGui::Checkbox("UseEffect", &material_->useEffect);
        const char* lights[] = { "Sepia", "Grayscale", "None" };
        int type_current = int(material_->type);

        if (ImGui::Combo("EffectType", &type_current, lights, IM_ARRAYSIZE(lights))) {
            material_->type = type_current % 3;
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

RenderTexture::~RenderTexture()
{
    if (materialResource_) {
        materialResource_->Unmap(0, nullptr);
        materialResource_ = nullptr;
    }

    materialResource_.Reset();
}

void RenderTexture::CreateMaterialBuffer()
{
    //マテリアル用のリソースを作る。
    materialResource_ = DirectXCommon::CreateBufferResource(sizeof(MaterialForRenderTexture));
    //マテリアルにデータを書き込む

    //書き込むためのアドレスを取得
    HRESULT result = materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material_));
    material_->color = sepiaColor_;
}

void RenderTexture::CreateMaterialBufferForVignette()
{    //マテリアル用のリソースを作る。
    materialResourceForVignette_ = DirectXCommon::CreateBufferResource(sizeof(MaterialForVignette));
    //マテリアルにデータを書き込む

    //書き込むためのアドレスを取得
    HRESULT result = materialResourceForVignette_->Map(0, nullptr, reinterpret_cast<void**>(&materialForVignette_));
    materialForVignette_->correctVal = 16.0f;
    materialForVignette_->viignetteVal = 0.8f;
}

void RenderTexture::CreateMaterialBufferForBoxFilter()
{
    materialResourceForBoxFilter_ = DirectXCommon::CreateBufferResource(sizeof(MaterialForBoxFilter));
    //マテリアルにデータを書き込む

    //書き込むためのアドレスを取得
    HRESULT result = materialResourceForBoxFilter_->Map(0, nullptr, reinterpret_cast<void**>(&materialForBoxFilter_));
    materialForBoxFilter_->kernel = 1.0f;
}
