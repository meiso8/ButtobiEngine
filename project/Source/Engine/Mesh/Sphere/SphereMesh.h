#pragma once

#include"MeshCommon.h"

class SphereMesh :public MeshCommon
{
public:
    ~SphereMesh();
    void Create(const Texture::TEXTURE_HANDLE& textureHandle);
    void Draw(ID3D12GraphicsCommandList* commandList)override;
    void SetUVScale(const Vector3& scale) { uvTransform_.scale = scale; };
    void UpdateUV();
    void SetVertex(const float& radius = 1.0f);
private:
    void CreateVertex()override;
    void CreateUV();

private:
    Transform uvTransform_ = { 0.0f };
    Matrix4x4 uvTransformMatrix_{};
    const uint32_t kSubdivision_ = 16;//分割数
};


