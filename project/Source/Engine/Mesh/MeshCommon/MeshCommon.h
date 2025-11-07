#pragma once
#include"VertexData.h"
#include<wrl.h>
#include<d3d12.h>
#include"ModelConfig.h"
#include"commandList.h"  
#include"Balloon.h"
#include"Wave.h"
#include"MaterialResource.h"  
#include"Transform.h"
#include"PSO.h"  

class MeshCommon
{
public:
    void Finalize();
    //三角面用
    virtual void PreDraw(ID3D12GraphicsCommandList* commandList, const LightMode& lightMode = LightMode::klightModeNone, const BlendMode& blendMode = BlendMode::kBlendModeNormal, const CullMode& cullMode = CullMode::kCullModeBack);
    virtual void Draw(ID3D12GraphicsCommandList* commandList) = 0;

    void SetColor(const Vector4& color);
    Material* GetMaterial() { return materialResource_->GetMaterial(); };
    Vector4& GetColor() { return materialResource_->GetMaterial()->color; };
    VertexData& GetVertexData(const uint32_t& index) {
        return vertexData_[index];
    }
    Balloon& GetBalloonData() {
        return *balloonData_;
    }
    Wave& GetWaveData(size_t index) { return waveData_[index]; };

    void InitWaveData();
    void InitBalloonData();
protected:
    /// @brief テクスチャハンドル
    uint32_t textureHandle_ = 0;
    //マテリアルリソース
    MaterialResource* materialResource_ = nullptr;
    //rootSignatureとdirectionalLight
    static ModelConfig* modelConfig_;

    //頂点データ
    Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource_{};
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    VertexData* vertexData_ = nullptr;
    //インデックスデータ
    D3D12_INDEX_BUFFER_VIEW  indexBufferView_{};
    Microsoft::WRL::ComPtr <ID3D12Resource> indexResource_{};
    uint32_t* indexData_ = nullptr;
    //膨張データ
    Microsoft::WRL::ComPtr<ID3D12Resource> expansionResource_;
    Balloon* balloonData_ = nullptr;
    //波データ
    Microsoft::WRL::ComPtr<ID3D12Resource> waveResource_;
    Wave* waveData_ = nullptr;

protected:
    virtual void CreateVertex() = 0;
    virtual void CreateIndexResource();
    void CreateMaterial(const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, uint32_t lightType = 0);
    void CreateWaveData();
    void CreateBalloonData();
};

