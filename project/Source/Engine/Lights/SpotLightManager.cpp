#include "SpotLightManager.h"
#include"DirectXCommon.h"
SpotLight* SpotLightManager::spotLightData_ = nullptr;
Microsoft::WRL::ComPtr <ID3D12Resource> SpotLightManager::spotLightResource_ = nullptr;
#include<numbers>
#include"DebugUI.h"
void SpotLightManager::Create()
{
    //平行光源用のResourceを作成する
    spotLightResource_ = DirectXCommon::CreateBufferResource(sizeof(SpotLight));
    //書き込むためのアドレスを取得
    spotLightResource_->Map(0, nullptr, reinterpret_cast<void**>(&spotLightData_));

    //デフォルト値はとりあえず以下のようにしておく   
    spotLightData_->color = { 1.0f,230.0f / 255.0f,200.0f / 255.0f,1.0f };
    spotLightData_->position = { 0.0f,0.0f,0.0f };//位置
    spotLightData_->intensity = { 25.0f };//輝度
    spotLightData_->direction = { 0.0f,0.0f,1.0f };//スポットライトの方向
    spotLightData_->distance = { 1.0f };//ライトの届く範囲
    spotLightData_->decay = 0.1f;
    spotLightData_->cosAngle = cosf(std::numbers::pi_v<float>/8.0f);//スポットライトの余弦
    spotLightData_->cosFalloffStart = 2.0f;

    //書き込み終了！
    spotLightResource_->Unmap(0, nullptr);
}

void SpotLightManager::Finalize()
{
    if (spotLightResource_) {
        spotLightResource_.Reset();
    }

}

void SpotLightManager::SetGraphicsRootConstantBufferView()
{
    //LightのCBufferの場所を設定
    DirectXCommon::GetCommandList()->SetGraphicsRootConstantBufferView(8, spotLightResource_->GetGPUVirtualAddress());

}
