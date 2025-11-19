#pragma once
#include"MeshCommon.h"
struct AABB;

class CubeMesh :public MeshCommon
{
public:
    ~CubeMesh();
    void SetMinMax(const AABB& aabb);
    void SetVertex(const Vector3(&vertices)[8]);
    //デフォルト値を設定
    void Create(const Texture::TEXTURE_HANDLE& textureHandle = Texture::UV_CHECKER);
    void Draw(ID3D12GraphicsCommandList* commandList)override;
    void UpdateUV();
    /// @brief UVのTransformを得る
/// @return Transform
    Transform& GetUVTransform() { return uvTransform_; }
protected:
    void CreateVertex()override;
    void CreateIndexResource()override;

    void CreateUV();
    Transform uvTransform_ = { 0.0f };
    Matrix4x4 uvTransformMatrix_{ 0.0f };
};

