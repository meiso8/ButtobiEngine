#pragma once

#include<d3d12.h>
#include<dxgi1_6.h>
#include<dxcapi.h>
//ComPtr(コムポインタ)
#include<wrl.h>
#include"RootSignature.h"
#include"InputLayout.h"
#include"CompileShader.h"
#include"BlendState.h"
#include"RasterizerState.h"
#include"Depth.h"//StencilTextureの作成関数　奥行き
#include<memory>
#include<array>



class PSO {
public:

    enum TopologyType {
        kTriangle,
        kLine,
        kPoint,
        TopologyTypes
    };

    enum EffectType {
        kEffectNone,
        kEffectGrayScale,
        kEffectVignette,
        kEffectBoxFilter,
        kEffectGaussianFilter,
        kCountOfEffect,

    };


    static Microsoft::WRL::ComPtr <ID3D12PipelineState>& GetGraphicsPipelineState(uint32_t blendMode,uint32_t cullMode ) {
        return graphicsPipelineStates_[blendMode][cullMode];
        ;
    }
    static Microsoft::WRL::ComPtr <ID3D12PipelineState>& GetGraphicsPipelineStatesSkinning(uint32_t blendMode, uint32_t cullMode) {
        return graphicsPipelineStatesSkinning_[blendMode][cullMode];
        ;
    }
    static Microsoft::WRL::ComPtr <ID3D12PipelineState>& GetGraphicsPipelineStateParticle(uint32_t blendMode) {
        return graphicsPipelineStatesParticle_[blendMode];
        ;
    }

    static Microsoft::WRL::ComPtr <ID3D12PipelineState>& GetGraphicsPipelineStateLine() {
        return graphicsPipelineStatesLine_;
        ;
    }
    static Microsoft::WRL::ComPtr <ID3D12PipelineState>& GetGraphicsPipelineStateSkyBox() {
        return graphicsPipelineStateSkyBox_;
        ;
    }

    static Microsoft::WRL::ComPtr <ID3D12PipelineState>& GetGraphicsPipelineStateSprite(uint32_t blendMode) {
        return graphicsPipelineStateSprite_[blendMode];
        ;
    }
    static Microsoft::WRL::ComPtr <ID3D12PipelineState>& GetGraphicsPipelineStateFont(uint32_t blendMode) {
        return graphicsPipelineStateFont_[blendMode];
        ;
    }

    static Microsoft::WRL::ComPtr <ID3D12PipelineState>& GetGraphicsPipelineStateOffScreen(uint32_t effectType){
        return graphicsPipelineStateOffScreen_[effectType];
    }

    void CreateALLPSO();
    static RootSignature* GetRootSignature() { return rootSignature.get(); }
    ~PSO();
private:
    Microsoft::WRL::ComPtr <ID3D12PipelineState> Create(
        RootSignature& rootSignature,
        InputLayout& inputLayout,
        BlendState& blendState,
        RasterizerState& rasterizerState,
        DepthStencil& depthStencil, const ShaderType shaderType, const TopologyType topologyType,const InputLayout::InputLayoutType inputLayoutType);
public:
    static std::unique_ptr<RootSignature>rootSignature;
private:
    static std::array<std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfCullMode>, kCountOfBlendMode> graphicsPipelineStates_;

    static std::array<std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfCullMode>, kCountOfBlendMode> graphicsPipelineStatesSkinning_;
    static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState> ,kCountOfBlendMode> graphicsPipelineStatesParticle_;
    static Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStatesLine_;
    static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfBlendMode> graphicsPipelineStateSprite_;
    static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfBlendMode> graphicsPipelineStateFont_;

    static Microsoft::WRL::ComPtr<ID3D12PipelineState> graphicsPipelineStateSkyBox_;
    static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCountOfEffect> graphicsPipelineStateOffScreen_;

    std::unique_ptr<InputLayout>inputLayout = nullptr;
    std::vector<BlendState> blendStates = {};
    std::vector<RasterizerState> rasterizerStates = {};
    std::vector<DepthStencil>  depthStencils = {};
};