#pragma once
#include"MeshCommon.h"
struct Circle;

class CircleMesh :public MeshCommon
{
public:
    ~CircleMesh();
    void Create(const TextureFactory::Handle& textureHandle = TextureFactory::UV_CHECKER);
    void Draw(ID3D12GraphicsCommandList* commandList)override;

    void SetVertex(const Circle& spehre);
private:
    void CreateVertex()override;

    void CreateIndexResource()override;
private:

    const uint32_t kSubdivision_ = 16;//分割数

};
