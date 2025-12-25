#pragma once

#include<d3d12.h>
#include<array>
#include<cassert>
class InputLayout
{
public:
    enum InputLayoutType {
        kInputLayoutTypeNormal,
        kInputLayoutTypeSkinning,
        kMaxInputLayoutType,
    };
    void Create();
    D3D12_INPUT_LAYOUT_DESC& GetDescs(const InputLayoutType& index) {
        assert(index < kMaxInputLayoutType);
        return inputLayoutDescs_[index];
   };
private:
     D3D12_INPUT_ELEMENT_DESC inputElementDescs_[4] = {};
  D3D12_INPUT_ELEMENT_DESC inputElementDescsSkinning_[5] = {};
   std::array< D3D12_INPUT_LAYOUT_DESC, kMaxInputLayoutType> inputLayoutDescs_{};
};

