#pragma once

#pragma region //自作関数
#include"D3DResourceLeakChecker.h"
#include"Input.h"
#include"DirectXCommon.h"
#include "RootSignature.h"

#include"PSO.h"
#include"Depth.h"//StencilTextureの作成関数　奥行き
#include"CompileShader.h"
#include"BlendState.h"
#include"RasterizerState.h"
#include"Texture.h"

#include"ModelManager.h"
#include"ModelConfig.h"

#include"SoundManager.h"
#include"Sound.h"

#include"CrashHandler.h"
#include"Log.h"
#include"InputLayout.h"
#include"DebugUI.h"

#include"DirectionalLight.h"
#include<memory>

#pragma endregion

class MyEngine {
public:
    MyEngine() = default;
    ~MyEngine() = default;
    MyEngine(MyEngine&) = delete;
    MyEngine& operator=(MyEngine&) = delete;
    static MyEngine* GetInstance();
    void Create(const std::wstring& title, const int32_t clientWidth, const int32_t clientHeight);
    void Update();
    void PreCommandSet(Vector4& screenColor);
    void PostCommandSet();
    void Finalize();

    static Window& GetWC() { return *wc; };
    static RootSignature* GetRootSignature() { return rootSignature.get(); }
    static PSO* GetPSO(uint32_t index) { return &pso[index]; }
    static DirectionalLight* GetDirectionalLightData() { return directionalLightData; }
    static void SetBlendMode(uint32_t blendMode = BlendMode::kBlendModeNormal);
public:

private:
    D3DResourceLeakChecker leakCheck = {};
    static MyEngine* instance_;

    std::unique_ptr<DirectXCommon> directXCommon = nullptr;
    std::unique_ptr<LogFile> logFile = nullptr;
    static std::unique_ptr<Window> wc;

    Input* input = nullptr;
    //音声クラスの作成

    Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource = nullptr;
    static DirectionalLight* directionalLightData;

    static std::array<PSO, kCountOfBlendMode> pso;
    static std::unique_ptr<RootSignature>rootSignature;
    static ModelConfig modelConfig_;
    std::unique_ptr<InputLayout>inputLayout = nullptr;
    std::vector<BlendState> blendStates = {};
    std::vector<RasterizerState> rasterizerStates = {};
    DepthStencil depthStencil = {};
};

