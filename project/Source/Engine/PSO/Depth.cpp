#include"Depth.h"

void DepthStencil::Create(MaskMode maskMode) {
    //Depthの機能を有効化する
    depthStencilDesc_.DepthEnable = true;

    if (maskMode == MaskMode::kZero) {
        //書き込みします
        depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    } else if(maskMode == MaskMode::kAll){
        //書き込みします
        depthStencilDesc_.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    }

    //比較関数はLessEqual。つまり、近ければ描画される
    depthStencilDesc_.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
};
