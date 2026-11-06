#include "GPU.h"
#include"Log.h"

#include<cassert>
#include<format>
#include"StringUtility.h"

void GPU::SettingGPU(DXGIFactory& dxgiFactory) {

    //良い順にアダプタを頼む
    for (UINT i = 0;   dxgiFactory.GetDxgiFactory()->EnumAdapterByGpuPreference(i,
        DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) !=
        DXGI_ERROR_NOT_FOUND; ++i) {
        //アダプタの情報を取得する
        DXGI_ADAPTER_DESC3 adapterDesc{};
        HRESULT result = useAdapter_->GetDesc3(&adapterDesc);
        assert(SUCCEEDED(result));//取得できないのは一大事

        //ソフトウェアアダプタでなければ採用！
        if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
            //採用したアダプタ情報をログに出力。wstringの方なので注意
            LogFile::Log(StringUtility::ConvertString(std::format(L"Use Adapter:{}\n", adapterDesc.Description)));
            break;
        }

        useAdapter_ = nullptr;//ソフトウェアアダプタの場合は見なかったことにする

    }

    //適切なアダプタが見つからなかったので起動できない
    assert(useAdapter_ != nullptr);

}

