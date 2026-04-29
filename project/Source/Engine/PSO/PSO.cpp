
#include "PSO.h"
#include"Log.h"

#include<cassert>

#include"DirectXCommon.h"

std::unique_ptr<RootSignature>PSO::rootSignature = nullptr;
std::array<std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfCullMode>, kCountOfBlendMode>PSO::graphicsPipelineStates_;
std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfBlendMode>PSO::graphicsPipelineStatesParticle_;
Microsoft::WRL::ComPtr<ID3D12PipelineState> PSO::graphicsPipelineStatesLine_;
std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfBlendMode> PSO::graphicsPipelineStateSprite_;
std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfBlendMode> PSO::graphicsPipelineStateFont_;
Microsoft::WRL::ComPtr<ID3D12PipelineState> PSO::graphicsPipelineStateSkyBox_;
std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, PSO::kCountOfEffect> PSO::graphicsPipelineStateOffScreen_;
std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfBlendMode> PSO::graphicsPipelineStateRandom_;
std::array<std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfCullMode>, kCountOfBlendMode>PSO::graphicsPipelineStatesSkinning_;


Microsoft::WRL::ComPtr <ID3D12PipelineState> PSO::Create(
    InputLayout& inputLayout,
    BlendState& blendState,
    RasterizerState& rasterizerState,
    DepthStencil& depthStencil,
    bool useDepthFormat,
    const RootSignature::TYPE& rootSignatureType,
    const DxcCompiler::VS_TYPE& vsShaderType,
    const  DxcCompiler::PS_TYPE& psShaderType,
    const TopologyType topologyType,
    const InputLayout::InputLayoutType inputLayoutType ) {

    D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

    graphicsPipelineStateDesc.pRootSignature = rootSignature->GetRootSignature(rootSignatureType);//RootSignature
    graphicsPipelineStateDesc.InputLayout = inputLayout.GetDescs(inputLayoutType);//InputLayout
    graphicsPipelineStateDesc.VS = { DirectXCommon::GetDxcCompiler()->GetVertexShaderBlob(vsShaderType)->GetBufferPointer(),
    DirectXCommon::GetDxcCompiler()->GetVertexShaderBlob(vsShaderType)->GetBufferSize() };//VertexShader
    graphicsPipelineStateDesc.PS = { DirectXCommon::GetDxcCompiler()->GetPixelShaderBlob(psShaderType)->GetBufferPointer(),
    DirectXCommon::GetDxcCompiler()->GetPixelShaderBlob(psShaderType)->GetBufferSize() };//PixelShader
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
    if (useDepthFormat) {
        graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
    } else {
        graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
    }


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
    depthStencils[kNone].Create(kNone);

    LogFile::Log("Create depthStencilDesc");

    for (uint32_t b = 0; b < kCountOfBlendMode; ++b) {
        for (uint32_t c = 0; c < kCountOfCullMode; ++c) {
            graphicsPipelineStates_[b][c] = Create(
                *inputLayout,
                blendStates[b],
                rasterizerStates[c],
                depthStencils[kAll],
                true,
                RootSignature::NORMAL,
                DxcCompiler::VS_Normal,
                DxcCompiler::PS_Normal,
                kTriangle,
                InputLayout::kInputLayoutTypeNormal
            );
        }
    }

    for (int b = 0; b < kCountOfBlendMode; ++b) {
        graphicsPipelineStatesParticle_[b] = Create(  
            *inputLayout,
            blendStates[b],
            rasterizerStates[kCullModeNone],
            depthStencils[kZero],
            true, 
            RootSignature::PARTICLE,
            DxcCompiler::VS_Particle,
            DxcCompiler::PS_Particle,
            kTriangle, 
            InputLayout::kInputLayoutTypeNormal
        );
    }


    graphicsPipelineStatesLine_ =
        Create(
           *inputLayout,
            blendStates[kBlendModeNone],
            rasterizerStates[kCullModeBack],
            depthStencils[kAll],
            true,
            RootSignature::NORMAL,
            DxcCompiler::VS_Normal,
            DxcCompiler::PS_Normal,
            kLine,
            InputLayout::kInputLayoutTypeNormal
        );

    for (int b = 0; b < kCountOfBlendMode; ++b) {
        graphicsPipelineStateSprite_[b] = Create(
            *inputLayout,
            blendStates[b],
            rasterizerStates[kCullModeBack],
            depthStencils[kNone], 
            false,
            RootSignature::SPRITE,
            DxcCompiler::VS_Sprite,
            DxcCompiler::PS_Sprite,
            kTriangle,
            InputLayout::kInputLayoutTypeNormal
        );

    }

    for (int b = 0; b < kCountOfBlendMode; ++b) {
        graphicsPipelineStateFont_[b] = Create(
            *inputLayout,
            blendStates[b],
            rasterizerStates[kCullModeBack],
            depthStencils[kNone],
            false,
            RootSignature::FONT,
            DxcCompiler::VS_Sprite,
            DxcCompiler::PS_Font,
            kTriangle, 
            InputLayout::kInputLayoutTypeNormal
        );
    }


    for (uint32_t b = 0; b < kCountOfBlendMode; ++b) {
        for (uint32_t c = 0; c < kCountOfCullMode; ++c) {
            graphicsPipelineStatesSkinning_[b][c] = Create(
                *inputLayout,
                blendStates[b],
                rasterizerStates[c],
                depthStencils[kAll],
                true,
                RootSignature::SKINNING,
                DxcCompiler::VS_Skinning,
                DxcCompiler::PS_Normal,
                kTriangle, 
                InputLayout::kInputLayoutTypeSkinning
            );
        }
    }

    graphicsPipelineStateSkyBox_ =
        Create(
            *inputLayout,
            blendStates[kBlendModeNone],
            rasterizerStates[kCullModeNone],
            depthStencils[kZero],
            true,
            RootSignature::SKYBOX,
            DxcCompiler::VS_SkyBox,
            DxcCompiler::PS_SkyBox,
            kTriangle,
            InputLayout::kInputLayoutTypeNormal);


    graphicsPipelineStateOffScreen_[kEffectNone] = Create(
        *inputLayout,
        blendStates[kBlendModeNone],
        rasterizerStates[kCullModeBack],
        depthStencils[kNone],
        false,
        RootSignature::OFFSCREEN,
        DxcCompiler::VS_OffScreen,
        DxcCompiler::PS_OffScreen,
        kTriangle,
        InputLayout::kInputLayoutTypeOffScreen);

    graphicsPipelineStateOffScreen_[kEffectGrayScale] = Create(
        *inputLayout,
        blendStates[kBlendModeNone],
        rasterizerStates[kCullModeBack],
        depthStencils[kNone],
        false,
        RootSignature::GRAYSCALE,
        DxcCompiler::VS_OffScreen,
        DxcCompiler::PS_GrayScale,
        kTriangle,
        InputLayout::kInputLayoutTypeOffScreen);

    graphicsPipelineStateOffScreen_[kEffectVignette] = Create(
        *inputLayout,
        blendStates[kBlendModeNone],
        rasterizerStates[kCullModeBack],
        depthStencils[kNone],
        false,
        RootSignature::VIGNETTE,
        DxcCompiler::VS_OffScreen,
        DxcCompiler::PS_Vignette,
        kTriangle,
        InputLayout::kInputLayoutTypeOffScreen);

    graphicsPipelineStateOffScreen_[kEffectBoxFilter] = Create(
        *inputLayout,
        blendStates[kBlendModeNone],
        rasterizerStates[kCullModeBack],
        depthStencils[kNone],
        false,
        RootSignature::BOXFILTER,
        DxcCompiler::VS_OffScreen,
        DxcCompiler::PS_BoxFilter,
        kTriangle,
        InputLayout::kInputLayoutTypeOffScreen);

    graphicsPipelineStateOffScreen_[kEffectGaussianFilter] = Create(
        *inputLayout,
        blendStates[kBlendModeNone],
        rasterizerStates[kCullModeBack],
        depthStencils[kNone],
        false,
        RootSignature::GAUSSIANFILTER,
        DxcCompiler::VS_OffScreen,
        DxcCompiler::PS_GaussianFilter,
        kTriangle,
        InputLayout::kInputLayoutTypeOffScreen);


    graphicsPipelineStateOffScreen_[kEffectLuminanceBasedOutline] = Create(
        *inputLayout,
        blendStates[kBlendModeNone],
        rasterizerStates[kCullModeBack],
        depthStencils[kNone],
        false,
        RootSignature::LUMINANCE_BASED_OUTLINE,
        DxcCompiler::VS_OffScreen,
        DxcCompiler::PS_LuminanceBasedOutline,
        kTriangle,
        InputLayout::kInputLayoutTypeOffScreen);

    graphicsPipelineStateOffScreen_[kEffectDepthBasedOutline] = Create(
        *inputLayout,
        blendStates[kBlendModeNone],
        rasterizerStates[kCullModeBack],
        depthStencils[kNone], 
        false,
        RootSignature::DEPTH_BASED_OUTLINE,
        DxcCompiler::VS_OffScreen,
        DxcCompiler::PS_DepthBasedOutline,
        kTriangle,
        InputLayout::kInputLayoutTypeOffScreen);


    graphicsPipelineStateOffScreen_[kEffectRadialBlur] = Create(
        *inputLayout,
        blendStates[kBlendModeNone],
        rasterizerStates[kCullModeBack],
        depthStencils[kNone],
        false,
        RootSignature::RADIAL_BLUR,
        DxcCompiler::VS_OffScreen,
        DxcCompiler::PS_RadialBlur,
        kTriangle,
        InputLayout::kInputLayoutTypeOffScreen);

    graphicsPipelineStateOffScreen_[kEffectDissolve] = Create(
        *inputLayout,
        blendStates[kBlendModeNone],
        rasterizerStates[kCullModeBack],
        depthStencils[kNone],
        false,
        RootSignature::DISSOLVE,
        DxcCompiler::VS_OffScreen,
        DxcCompiler::PS_Dissolve,
        kTriangle,
        InputLayout::kInputLayoutTypeOffScreen);

    for (int b = 0; b < kCountOfBlendMode; ++b) {
        graphicsPipelineStateRandom_[b] = Create(
            *inputLayout,
            blendStates[b],
            rasterizerStates[kCullModeBack],
            depthStencils[kNone],
            false,
            RootSignature::RANDOM,
            DxcCompiler::VS_OffScreen,
            DxcCompiler::PS_Random,
            kTriangle, 
            InputLayout::kInputLayoutTypeOffScreen
        );
    }
    
}

PSO::~PSO()
{

    for (auto& blendModes : graphicsPipelineStatesSkinning_) {
        for (auto& pso : blendModes) {
            if (pso) {
                pso.Reset(); // Release() と同じ効果
            }
        }
    }

    for (auto& pso : graphicsPipelineStateSprite_) {
        if (pso) {
            pso.Reset();
        }
    }

    for (auto& pso : graphicsPipelineStateFont_) {
        if (pso) {
            pso.Reset();
        }
    }

    if (graphicsPipelineStatesLine_) {
        graphicsPipelineStatesLine_.Reset();
    }


    for (auto& pso : graphicsPipelineStatesParticle_) {
        if (pso) {
            pso.Reset();
        }
    }


    for (auto& blendModes : graphicsPipelineStates_) {
        for (auto& pso : blendModes) {
            if (pso) {
                pso.Reset(); // Release() と同じ効果
            }
        }
    }

    if (graphicsPipelineStateSkyBox_) {
        graphicsPipelineStateSkyBox_.Reset();
    }

    rootSignature.reset();


}

