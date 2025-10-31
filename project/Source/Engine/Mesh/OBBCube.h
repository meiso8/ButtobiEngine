#pragma once
#include"MeshCommon.h"

class OBBCube :public MeshCommon
{
public:
    ~OBBCube();
    void SetVertex(const Vector3(&vertices)[8]);

    void Create(uint32_t& textureHandle);

    void Draw(Camera& camera, const Matrix4x4& worldMatrix, const uint32_t lightType = MaterialResource::LIGHTTYPE::NONE)override;
private:
    void CreateVertex()override;
    void CreateIndexResource()override;
};

