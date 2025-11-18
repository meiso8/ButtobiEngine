#pragma once
#include"MeshCommon.h"
struct Circle;

class CircleMesh :public MeshCommon
{
public:
    ~CircleMesh();
    void Create(const Texture::TEXTURE_HANDLE& textureHandle = Texture::UV_CHECKER);
    void Draw(ID3D12GraphicsCommandList* commandList)override;
    void SetUVScale(const Vector3& scale) { uvTransform_.scale = scale; };
    void SetUVTranslate(const Vector3& translate) { uvTransform_.translate = translate; };
    void SetUVRotate(const Vector3& rotate) { uvTransform_.rotate = rotate; };
    void UpdateUV();
    void SetVertex(const Circle& spehre);
private:
    void CreateVertex()override;
    void CreateUV();
    void CreateIndexResource()override;
private:
    Transform uvTransform_ = { 0.0f };
    Matrix4x4 uvTransformMatrix_{};
    const uint32_t kSubdivision_ = 16;//分割数

};
