#pragma once
#include"MeshCommon.h"


class QuadMesh :public MeshCommon
{
public:
    void Create(uint32_t textureHandle);
void Draw(ID3D12GraphicsCommandList* commandList)override;

    void ResetSize(const Vector2& size);
    void UpdateUV();
    Vector3& GetUVScale() { return uvTransform_.scale; };
    Vector3& GetUVRotate() { return uvTransform_.rotate; };
    Vector3& GetUVTranslate() { return uvTransform_.translate; };
    ~QuadMesh();
private:
    void CreateVertex()override;
    void CreateIndexResource()override;
    void CreateUVTransformationMatrix();

private:

    Transform uvTransform_ = { 0.0f };
    Matrix4x4 uvTransformMatrix_{};
    Vector2 size_ = { 1.0f,1.0f };

};

