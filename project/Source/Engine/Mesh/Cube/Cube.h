#pragma once
#include"MeshCommon.h"

class Cube :public MeshCommon
{
public:
    ~Cube();
    void SetMinMax(const Vector3& min, const Vector3& max);

    void Create(uint32_t& textureHandle);

    void Draw(ID3D12GraphicsCommandList* commandList)override;
private:
    void CreateVertex()override;
    void CreateIndexResource()override;
};

