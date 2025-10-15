#pragma once

#include<d3d12.h>
#include<wrl.h>
#include<cstdint>
#include<array>

class RootSignature
{
public:
   void Create();
   ID3D12RootSignature* GetRootSignature(uint32_t index) { return rootSignatures_[index].Get(); };

private:
    std::array<Microsoft::WRL::ComPtr <ID3D12RootSignature>,2> rootSignatures_ = {nullptr};
};

