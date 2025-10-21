
#include "PSO.h"
#include"Log.h"

#include<cassert>

#include"DirectXCommon.h"

std::unique_ptr<RootSignature>PSO::rootSignature = nullptr;
std::array<std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfCullMode>, kCountOfBlendMode>PSO::graphicsPipelineStates_;
std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfBlendMode>PSO::graphicsPipelineStatesParticle_;
Microsoft::WRL::ComPtr<ID3D12PipelineState> PSO::graphicsPipelineStatesLine_;
 std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfBlendMode> PSO::graphicsPipelineStateSprite_;

Microsoft::WRL::ComPtr <ID3D12PipelineState> PSO::Create(
    RootSignature& rootSignature,
    InputLayout& inputLayout,
    BlendState& blendState,
    RasterizerState& rasterizerState,
    DepthStencil& depthStencil,
    const ShaderType shaderType,
    const TopologyType topologyType) {

    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

    graphicsPipelineStateDesc.pRootSignature = rootSignature.GetRootSignature(shaderType);//RootSignature
    graphicsPipelineStateDesc.InputLayout = inputLayout.GetDesc();//InputLayout
    graphicsPipelineStateDesc.VS = { DirectXCommon::GetDxcCompiler()->GetVertexShaderBlob(shaderType)->GetBufferPointer(),
    DirectXCommon::GetDxcCompiler()->GetVertexShaderBlob(shaderType)->GetBufferSize() };//VertexShader
    graphicsPipelineStateDesc.PS = { DirectXCommon::GetDxcCompiler()->GetPixelShaderBlob(shaderType)->GetBufferPointer(),
    DirectXCommon::GetDxcCompiler()->GetPixelShaderBlob(shaderType)->GetBufferSize() };//PixelShader
    graphicsPipelineStateDesc.BlendState = blendState.GetDesc();//BlendState
    graphicsPipelineStateDesc.RasterizerState = rasterizerState.GetDesc();//RasterizerState
    //書き込むRTVの情報
    graphicsPipelineStateDesc.NumRenderTargets = 1;
    graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

    //どのように画面に色を打ち込むかの設定（気にしなくていい）
    graphicsPipelineStateDesc.SampleDesc.Count = 1;
    graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    //DepthStencilの設定   
    graphicsPipelineStateDesc.DepthStencilState = depthStencil.GetDesc();
    graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

    switch (topologyType)
    {
    case PSO::kTriangle:
        graphicsPipelineStateDesc.PrimitiveTopologyType =
            D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        break;
    case PSO::kLine:
        graphicsPipelineStateDesc.PrimitiveTopologyType =
            D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
        break;
    case PSO::kPoint:
        graphicsPipelineStateDesc.PrimitiveTopologyType =
            D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
        break;
    default:
        break;
    }

    Microsoft::WRL::ComPtr <ID3D12PipelineState>graphicsPipelineState;

    //実際に生成
    HRESULT hr = DirectXCommon::GetDevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
        IID_PPV_ARGS(&graphicsPipelineState));
    assert(SUCCEEDED(hr));

    return graphicsPipelineState;

}

void PSO::CreateALLPSO()
{
    //RootSignatureを生成する
    rootSignature = std::make_unique<RootSignature>();
    //具体的にShaderがどこかでデータを読めばいいのかの情報を取りまとめたもの
    rootSignature->Create();
    LogFile::Log("CreateRootSignature");

    //InputLayout
    inputLayout = std::make_unique<InputLayout>();
    inputLayout->Create();
    LogFile::Log("InputLayout");

    //BlendStateの設定を行う
    blendStates.resize(kCountOfBlendMode);
    for (int i = 0; i < blendStates.size(); ++i) {
        blendStates[i].Create(i);
    }
    LogFile::Log("SetBlendState");

    //RasterizerStateの設定を行う
    rasterizerStates.resize(kCountOfCullMode);
    rasterizerStates[kCullModeNone].Create(kCullModeNone, kFillModeSolid);//ソリッドモードカリングなし
    rasterizerStates[kCullModeFront].Create(kCullModeFront, kFillModeSolid);//ソリッドモード裏面
    rasterizerStates[kCullModeBack].Create(kCullModeBack, kFillModeSolid);//ソリッドモード表面
    //rasterizerStates[0].Create(kCullModeNone, kFillModeWireframe);//ワイヤーフレームモード
    LogFile::Log("SetRasterizerState");


    //DepthStencilStateの設定
    depthStencils.resize(kMasks);
    depthStencils[kZero].Create(kZero);
    depthStencils[kAll].Create(kAll);
    LogFile::Log("Create depthStencilDesc");

    for (uint32_t b = 0; b < kCountOfBlendMode; ++b) {
        for (uint32_t c = 0; c < kCountOfCullMode; ++c) {
            graphicsPipelineStates_[b][c] = Create(
                *rootSignature,
                *inputLayout,
                blendStates[b],
                rasterizerStates[c],
                depthStencils[kAll],
                kNormal,
                kTriangle
            );
        }
    }

    for (int b = 0; b < kCountOfBlendMode; ++b) {
        graphicsPipelineStatesParticle_[b] = Create(
            *rootSignature,
            *inputLayout,
            blendStates[b],
            rasterizerStates[kCullModeBack],
            depthStencils[kZero], kParticle, kTriangle
        );
    }


    graphicsPipelineStatesLine_ =
        Create(
            *rootSignature,
            *inputLayout,
            blendStates[kBlendModeNone],
            rasterizerStates[kCullModeBack],
            depthStencils[kAll],
            kNormal,
            kLine);

    for (int b = 0; b < kCountOfBlendMode; ++b) {
        graphicsPipelineStateSprite_[b] = Create(
            *rootSignature,
            *inputLayout,
            blendStates[b],
            rasterizerStates[kCullModeBack],
            depthStencils[kAll], kSprite, kTriangle
        );
    }

}

