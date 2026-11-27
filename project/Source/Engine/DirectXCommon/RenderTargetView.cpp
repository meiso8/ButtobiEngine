#include "RenderTargetView.h"
#include<cassert>

#include"RtvDescriptorHeap.h"
#include"Device/Device.h"
#include"Log.h"


namespace RenderTargetView {
    //Descriptorは必ずDescriptorHandleというポインタのようなものを介して扱う必要がある
    //Viewを作るときは、どこのDescriptorに情報を格納するかを明示的に指定する必要がる
    //出力結果をSRGBに変換して書き込む //2dテクスチャとして書き込む
    const D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_ = { .Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB ,.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D };

    const D3D12_RENDER_TARGET_VIEW_DESC& GetDesc()  {
        return rtvDesc_;
    };

    void Create(Device& device, const D3D12_RENDER_TARGET_VIEW_DESC& rtvDesc, RenderTextureResource& resource, RtvDescriptorHeap* rtvDescriptorHeap)
    {
        uint32_t rtvIndex = rtvDescriptorHeap->Allocate();
        resource.rtvHandleCPU = rtvDescriptorHeap->GetCPUDescriptorHandle(rtvIndex);
        device.GetDevice()->CreateRenderTargetView(resource.Get(), &rtvDesc, resource.rtvHandleCPU);

    }
}