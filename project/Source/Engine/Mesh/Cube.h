#pragma once
#include"MeshCommon.h"

class Cube :public MeshCommon
{
public:
    ~Cube();
    void SetMinMax(const Vector3& min, const Vector3& max);

    void Create(uint32_t& textureHandle);

    void Draw(Camera& camera, const Matrix4x4& worldMatrix, const uint32_t lightType = MaterialResource::LIGHTTYPE::NONE)override;
private:
    void CreateVertex()override;
    void CreateIndexResource()override;
};

