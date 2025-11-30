#include "PointLightManager.h"
#include"DirectXCommon.h"

Microsoft::WRL::ComPtr <ID3D12Resource> PointLightManager::pointLightResource_;
PointLight* PointLightManager::pointLightData_;

void PointLightManager::CreateData()
{

    pointLightResource_ = DirectXCommon::CreateBufferResource(sizeof(PointLight
        ) * kMaxData_);
    //書き込むためのアドレスを取得
    pointLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&pointLightData_));

    //データを初期化する
    InitDatas();
}

void PointLightManager::SetGraphicsRootConstantBufferView()
{
    //pointLightのCBufferの場所を設定
    DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(7, pointLightResource_->GetGPUVirtualAddress());
}

void PointLightManager::InitData(const uint32_t& index)
{
    pointLightData_[index].color = { 1.0f,0.0f,0.0f,1.0f };
    pointLightData_[index].intensity = 1.0f;
    pointLightData_[index].position = { 0.0f,0.0f,0.0f };
}

void PointLightManager::InitDatas()
{
    for (uint32_t i = 0; i < kMaxData_; ++i) {
        InitData(i);
    }
}

void PointLightManager::Finalize()
{
    if (pointLightResource_ != nullptr) {
        pointLightResource_->Unmap(0, nullptr);
        pointLightResource_.Reset();
        pointLightResource_ = nullptr;
    }
}
