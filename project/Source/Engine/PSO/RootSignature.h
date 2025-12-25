#pragma once

#include<d3d12.h>
#include<wrl.h>
#include<cstdint>
#include<array>
#include<cassert>
class RootSignature
{
public:

    enum TYPE {
        NORMAL,
        PARTICLE,
        SPRITE,
        SKINNING,
        TYPES
    };

   void Create();
   ID3D12RootSignature* GetRootSignature(uint32_t index) {
       assert(index < rootSignatures_.size());
       return
       rootSignatures_[index].Get(); };

private:
    std::array<Microsoft::WRL::ComPtr <ID3D12RootSignature>, TYPES> rootSignatures_ = {nullptr};
};

