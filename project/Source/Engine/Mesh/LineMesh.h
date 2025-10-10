#pragma once

#include"MeshCommon.h"

class LineMesh :public MeshCommon
{
public:
    void Create(uint32_t& textureHandle);
    void PreDraw(const BlendMode& blendMode = BlendMode::kBlendModeNormal)override;
    void Draw(Camera& camera, const Matrix4x4& worldMatrix, const uint32_t lightType = MaterialResource::LIGHTTYPE::NONE)override;

    void SetVertexData(const Vector3& start, const Vector3& end);

private:
    void CreateVertex()override;
};

