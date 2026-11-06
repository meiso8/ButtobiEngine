#include "BaseDescriptor.h"
#include"Log.h"
#include<cassert>
#include"DescriptorHandle/DescriptorHandle.h"
#include"DirectXCommon.h"
#include"Device/Device.h"

Device* BaseDescriptor::device_ = nullptr;
BaseDescriptor::BaseDescriptor()
{
}

BaseDescriptor::~BaseDescriptor()
{
}

uint32_t BaseDescriptor::Allocate()
{
    assert(CanUseIndex());
    int index = useIndex_;
    useIndex_++;
    return index;
}

bool BaseDescriptor::CanUseIndex()
{
    if (useIndex_ < maxCount_) {
        return true;
    }

    LogFile::Log("Max Descriptor Count!\n");
    return false;
}

D3D12_CPU_DESCRIPTOR_HANDLE BaseDescriptor::GetCPUDescriptorHandle(uint32_t index)
{
    assert(CanUseIndex());
    return DescriptorHandle::GetCPUDescriptorHandle(descriptorHeap_.Get(), descriptorSize_, index);
}

D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor::GetGPUDescriptorHandle(uint32_t index)
{
    assert(CanUseIndex());
    return  DescriptorHandle::GetGPUDescriptorHandle(descriptorHeap_.Get(), descriptorSize_, index);
}

void BaseDescriptor::CreateDescriptorHeapAndSize(const D3D12_DESCRIPTOR_HEAP_TYPE& type, bool shaderVisible, const uint32_t kMaxCount)
{
    maxCount_ = kMaxCount;

    //DescriptorHeapを生成する
    if (descriptorHeap_ == nullptr) {
        descriptorHeap_ = CreateDescriptorHeap(type, maxCount_, shaderVisible);
    }

    assert(device_);
    descriptorSize_ = device_->GetDevice()->GetDescriptorHandleIncrementSize(type);

    LogFile::Log("Create DescriptorHeap And Size\n");
}


Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> BaseDescriptor::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
    if (!device_) {
        return nullptr;
    }
    //ディスクリプタヒープを生成する
    Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{};
    descriptorHeapDesc.Type = heapType;//レンダーターゲットビュー用
    descriptorHeapDesc.NumDescriptors = numDescriptors;
    descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    HRESULT hr = device_->GetDevice()->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
    //ディスクリプタヒープが作れなかったので起動できない
    assert(SUCCEEDED(hr));
    return descriptorHeap;
}