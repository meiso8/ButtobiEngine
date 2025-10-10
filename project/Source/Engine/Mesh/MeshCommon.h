#pragma once

#include"VertexData.h"
#include<wrl.h>
#include<d3d12.h>
#include"ModelConfig.h"
#include"commandList.h"  
#include"Balloon.h"
#include"Wave.h"
#include"MaterialResource.h"  
#include"TransformationMatrix.h"  
#include"PSO.h"  
#include"Camera/Camera.h"  

class MeshCommon
{
public:
    virtual void Create(uint32_t& textureHandle);
    virtual void PreDraw(BlendMode blendMode = BlendMode::kBlendModeNormal);
    virtual void Draw(Camera& camera, const Matrix4x4& worldMatrix, uint32_t lightType = MaterialResource::LIGHTTYPE::NONE);

    void SetColor(const Vector4& color);
    Material* GetMaterial() { return materialResource_.GetMaterial(); };
    VertexData& GetVertexData(const uint32_t& index) {
        return vertexData_[index];
    }
    Balloon& GetExpansionData() {
        return *expansionData_;
    }

protected:
    uint32_t textureHandle_ = 0;
    MaterialResource materialResource_{};
    static ModelConfig* modelConfig_;

    ID3D12GraphicsCommandList* commandList_ = nullptr;
    Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource_{};
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    VertexData* vertexData_ = nullptr;
    D3D12_INDEX_BUFFER_VIEW  indexBufferView_{};
    Microsoft::WRL::ComPtr <ID3D12Resource> indexResource_{};
    uint32_t* indexData_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> expansionResource_;
    Balloon* expansionData_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> waveResource_;
    Wave* waveData_ = nullptr;

    Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResource_ = nullptr;

    Matrix4x4 worldViewProjectionMatrix_{};
    TransformationMatrix* transformationMatrixData_ = nullptr;
protected:
   virtual void CreateVertex();
    virtual void CreateIndexResource();
    void CreateTransformationMatrix();
    void CreateMaterial(const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, uint32_t lightType = 0);
    void CreateWaveData();
    void CreateBalloonData();
};

