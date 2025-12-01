#pragma once
#include"VertexData.h"
#include<wrl.h>
#include<d3d12.h>
#include"commandList.h"  

#include"Transform.h"
#include"PSO.h"  
#include"Texture.h"

class MeshCommon
{
public:
    void Finalize();
    //三角面用
    virtual void PreDraw(ID3D12GraphicsCommandList* commandList, const BlendMode& blendMode = BlendMode::kBlendModeNormal,const CullMode& cullMode = CullMode::kCullModeBack);
    virtual void Draw(ID3D12GraphicsCommandList* commandList) = 0;

    VertexData& GetVertexData(const uint32_t& index) {
        return vertexData_[index];
    }
    void SetTextureHandle(const Texture::TEXTURE_HANDLE& textureHandle);
    uint32_t GetTextureHandle() { return textureHandle_; }
protected:
    /// @brief テクスチャハンドル
    uint32_t textureHandle_ = 0;

    //頂点データ
    Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource_{};
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    VertexData* vertexData_ = nullptr;
    //インデックスデータ
    D3D12_INDEX_BUFFER_VIEW  indexBufferView_{};
    Microsoft::WRL::ComPtr <ID3D12Resource> indexResource_{};
    uint32_t* indexData_ = nullptr;

protected:
    virtual void CreateVertex() = 0;
    virtual void CreateIndexResource();
};

