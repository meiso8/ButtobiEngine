#include "InputLayout.h"


void InputLayout::Create() {


    inputElementDescs_[0].SemanticName = "POSITION";
    inputElementDescs_[0].SemanticIndex = 0;
    inputElementDescs_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//RGBA
    inputElementDescs_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    inputElementDescs_[1].SemanticName = "TEXCOORD";
    inputElementDescs_[1].SemanticIndex = 0;
    inputElementDescs_[1].Format = DXGI_FORMAT_R32G32_FLOAT;//Vector2のためRG
    inputElementDescs_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
    inputElementDescs_[2].SemanticName = "NORMAL";
    inputElementDescs_[2].SemanticIndex = 0;
    inputElementDescs_[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;//RGB
    inputElementDescs_[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    //float3 worldPosition : POSITION0;
    inputElementDescs_[3].SemanticName = "POSITION";
    inputElementDescs_[3].SemanticIndex = 1;
    inputElementDescs_[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;//RGB
    inputElementDescs_[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;


    inputLayoutDescs_[kInputLayoutTypeNormal].pInputElementDescs = inputElementDescs_;
    inputLayoutDescs_[kInputLayoutTypeNormal].NumElements = _countof(inputElementDescs_);

    // =================================================================================================
    inputElementDescsSkinning_[0] = inputElementDescs_[0];
    inputElementDescsSkinning_[1] = inputElementDescs_[1];
    inputElementDescsSkinning_[2] = inputElementDescs_[2];

    inputElementDescsSkinning_[3].SemanticName = "WEIGHT";
    inputElementDescsSkinning_[3].SemanticIndex = 0;
    inputElementDescsSkinning_[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//RGBA float32_t4
    inputElementDescsSkinning_[3].InputSlot = 1;//1番目のslotのVBVのことだと伝える
    inputElementDescsSkinning_[3].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    inputElementDescsSkinning_[4].SemanticName = "INDEX";
    inputElementDescsSkinning_[4].SemanticIndex = 0;
    inputElementDescsSkinning_[4].Format = DXGI_FORMAT_R32G32B32A32_SINT;// int32_t4 詳しくはstruct VertexInfluenceを見ること
    inputElementDescsSkinning_[4].InputSlot = 1;//1番目のslotのVBVのことだと伝える
    inputElementDescsSkinning_[4].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

    inputLayoutDescs_[kInputLayoutTypeSkinning].pInputElementDescs = inputElementDescsSkinning_;
    inputLayoutDescs_[kInputLayoutTypeSkinning].NumElements = _countof(inputElementDescsSkinning_);

    // ============================================================================================
    //オフスクリーンレンダリングは頂点シェーダーを使用しないため、入力レイアウトは必要ない
    inputLayoutDescs_[kInputLayoutTypeOffScreen].pInputElementDescs = nullptr;
    inputLayoutDescs_[kInputLayoutTypeOffScreen].NumElements = 0;

}