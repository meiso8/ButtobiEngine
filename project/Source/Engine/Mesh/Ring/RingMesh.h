#pragma once
#include"MeshCommon.h"
struct Circle;

class RingMesh :public MeshCommon
{
public:
    ~RingMesh();
    void Create(const TextureFactory::Handle& textureHandle = TextureFactory::UV_CHECKER);
    void Draw(ID3D12GraphicsCommandList* commandList)override;

private:
    void SetVertex();
    void CreateVertex()override;
    void CreateIndexResource()override;
private:
    const uint32_t kRingDivide_ = 32;//分割数
    const float kInnerRadius_ = 0.2f;//内半径
    const float kOuterRadius_ = 1.0f;//外半径
};
