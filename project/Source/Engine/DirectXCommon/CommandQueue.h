#pragma once

#include<d3d12.h>
#include<dxgi1_6.h>

//ComPtr(コムポインタ)
#include<wrl.h>
class Device;
class CommandQueue
{
public:
    CommandQueue(Device& device);
    ~CommandQueue();
   const Microsoft::WRL::ComPtr<ID3D12CommandQueue>& GetCommandQueue() {
        return commandQueue_;};
private:
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
    D3D12_COMMAND_QUEUE_DESC commandQueueDesc_{};

};

