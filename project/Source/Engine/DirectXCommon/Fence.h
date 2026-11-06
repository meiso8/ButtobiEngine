#pragma once
#include<wrl.h>
#include<d3d12.h>
#include<cstdint>

class CommandQueue;
class Device;
class Fence
{
public:
    Fence(Device& device);
    ~Fence();
    Microsoft::WRL::ComPtr<ID3D12Fence>& GetFence() {
        return fence_
            ;
    };
  
    uint64_t GetValue() {
        return fenceValue_
            ;
    };
    void SendSignal(CommandQueue& commandQueue);
private:
    //初期値0でFenceを作る
    Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
    uint64_t fenceValue_ = 0;
};

