#include "CommandQueue.h"

#include<cassert> //assertも利用するため
#include"Device/Device.h"

CommandQueue::CommandQueue(Device& device)
{    //コマンドキューを生成する まとまった命令群を送るため
    HRESULT result = device.GetDevice()->CreateCommandQueue(&commandQueueDesc_, IID_PPV_ARGS(&commandQueue_));

    //コマンドキューの生成が上手くいかなかったので起動できない
    assert(SUCCEEDED(result));
}

CommandQueue::~CommandQueue()
{
}
