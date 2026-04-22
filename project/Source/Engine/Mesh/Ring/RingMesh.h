#pragma once
#include"MeshCommon.h"
struct Circle;

class RingMesh :public MeshCommon
{
public:
    ~RingMesh();
    void Create(const TextureFactory::Handle& textureHandle = TextureFactory::UV_CHECKER);
    void Draw(ID3D12GraphicsCommandList* commandList)override;
    void SetVertex(const float innerRadius = 0.2f, const float outerRadius = 1.0f);
private:
    const uint32_t kRingDivide_ = 32;//分割数
private:
   
    void CreateVertex()override;
    void CreateIndexResource()override;

};
