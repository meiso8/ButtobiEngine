#pragma once
#include<d3d12.h>
#include"DirectXCommon.h"
#include"commandList.h"
#include"RootSignature.h"
#include"VertexData.h"
class PSO;

class SpriteCommon
{
private:
    static RootSignature* rootSignature_;
    static D3D12_INDEX_BUFFER_VIEW  indexBufferView_;
    static Microsoft::WRL::ComPtr <ID3D12Resource> indexResource_;
    static uint32_t* indexData_;

public:

    static void Finalize();
     static void Initialize();
     static void SetIndexBuffer(ID3D12GraphicsCommandList* commandList);
     static void PreDraw(ID3D12GraphicsCommandList* commandList);
     static void DrawCall(ID3D12GraphicsCommandList* commandList);
private:
    static void CreateIndexResource();
};

