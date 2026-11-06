#pragma once
#include"wrl.h"
#include<d3d12.h>
class GPU;

class Device
{
private:
    Microsoft::WRL::ComPtr<ID3D12Device> device_ = nullptr;
public:
    Device(GPU&gpu);
    ~Device();
    Microsoft::WRL::ComPtr<ID3D12Device> CreateD3D12Device(GPU& gpu);
    const Microsoft::WRL::ComPtr<ID3D12Device>& GetDevice() { return device_; }
};

