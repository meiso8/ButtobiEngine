#pragma once

#include<d3d12.h>
#include<cstdint>//int32_tを使うため
//ComPtr(コムポインタ)
#include<wrl.h>
#include<array>
#include"ResourceManager/ResourceManager.h"

class RtvDescriptorHeap;
class Device;

namespace RenderTargetView {
    void Create(Device& device, const D3D12_RENDER_TARGET_VIEW_DESC& rtvDesc, RenderTextureResource& resource, RtvDescriptorHeap* rtvDescriptorHeap);
    const D3D12_RENDER_TARGET_VIEW_DESC& GetDesc();
}
