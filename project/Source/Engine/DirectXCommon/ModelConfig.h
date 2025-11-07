#pragma once
#include"commandList.h"
#include"PSO/RootSignature.h"
#include<wrl.h>

class ModelConfig {
private:
    static ModelConfig* instance_;
public:
    static RootSignature* rootSignature;
    static ID3D12Resource* directionalLightResource;//共通のライトリソース
private:
public:
    static ModelConfig* GetInstance();
    void Initialize(RootSignature* rootSignature, const Microsoft::WRL::ComPtr <ID3D12Resource> directionalLightResource);
    static void Finalize();
};
