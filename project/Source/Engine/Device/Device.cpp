#include "Device.h"
#include"GPU.h"
#include"Log.h"
#include<format>
#include<cassert>

Device::Device(GPU& gpu)
{
    //D3D12Deviceの生成
    device_ = CreateD3D12Device(gpu);
    //ファイルへのログ出力
    LogFile::Log("Complete create D3D12Device!!!\n");//初期化完了のログを出す
}

Device::~Device()
{
    if (device_) {
        device_.Reset();
    }
}

Microsoft::WRL::ComPtr<ID3D12Device> Device::CreateD3D12Device(GPU& gpu) {

    Microsoft::WRL::ComPtr<ID3D12Device> device = nullptr;

    //機能レベル(FEATURE_LEVEL)とログの出力用の文字列
    D3D_FEATURE_LEVEL featureLevels[] = {
      D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0
    };

    const char* featureLevelStrings[] = { "12.2","12.1","12.0" };
    //高い順に生成出来るか試していく
    for (size_t i = 0; i < _countof(featureLevels); ++i) {
        //採用したアダプターでデバイスを生成
        HRESULT hr = D3D12CreateDevice(gpu.GetUseAdapter().Get(), featureLevels[i], IID_PPV_ARGS(device.ReleaseAndGetAddressOf()));
        //指定した機能レベルでデバイスが生成できたかを確認する
        if (SUCCEEDED(hr)) {

            LogFile::Log(std::format("FeatureLevel : {}\n", featureLevelStrings[i]));
            break;
        }
    }

    //デバイスの生成が上手くいかなかったので起動できない
    assert(device != nullptr);

    return device;

}
