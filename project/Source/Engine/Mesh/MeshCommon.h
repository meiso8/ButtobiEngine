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
    void Finalize();
    virtual void PreDraw(const BlendMode& type = BlendMode::kBlendModeNormal, const CullMode& cullMode = CullMode::kCullModeBack);
    virtual void Draw(Camera& camera, const Matrix4x4& worldMatrix, const uint32_t lightType = MaterialResource::LIGHTTYPE::NONE) = 0;

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
    uint32_t textureHandle_ = 0;
    MaterialResource* materialResource_ = nullptr;

    static ModelConfig* modelConfig_;
    static  ID3D12GraphicsCommandList* commandList_;

    Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource_{};
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    VertexData* vertexData_ = nullptr;
    D3D12_INDEX_BUFFER_VIEW  indexBufferView_{};
    Microsoft::WRL::ComPtr <ID3D12Resource> indexResource_{};
    uint32_t* indexData_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> expansionResource_;
    Balloon* balloonData_ = nullptr;

    Microsoft::WRL::ComPtr<ID3D12Resource> waveResource_;
    Wave* waveData_ = nullptr;

    Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResource_ = nullptr;

    Matrix4x4 worldViewProjectionMatrix_{};
    TransformationMatrix* transformationMatrixData_ = nullptr;
protected:
    virtual void CreateVertex() = 0;
    virtual void CreateIndexResource();
    void CreateTransformationMatrix();
    void CreateMaterial(const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, uint32_t lightType = 0);
    void CreateWaveData();
    void CreateBalloonData();
};

