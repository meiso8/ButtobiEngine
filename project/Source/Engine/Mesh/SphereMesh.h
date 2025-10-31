#pragma once

#include"MeshCommon.h"

class SphereMesh :public MeshCommon
{
public:
    ~SphereMesh();
    void Create(uint32_t textureHandle);
    void Draw(Camera& camera, const Matrix4x4& worldMatrix, uint32_t lightType = MaterialResource::LIGHTTYPE::NONE)override;
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


