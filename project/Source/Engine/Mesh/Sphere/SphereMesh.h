#pragma once

#include"MeshCommon.h"
#include"Sphere.h"

class SphereMesh :public MeshCommon
{
public:
    ~SphereMesh();
    void Create(const Texture::TEXTURE_HANDLE& textureHandle = Texture::UV_CHECKER);
    void Draw(ID3D12GraphicsCommandList* commandList)override;
    void SetVertex(const Sphere& spehre);
private:
    void CreateVertex()override;
private:

    const uint32_t kSubdivision_ = 16;//分割数
};


