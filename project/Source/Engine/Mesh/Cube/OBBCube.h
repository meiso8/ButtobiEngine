#pragma once
#include"MeshCommon.h"

class OBBCube :public MeshCommon
{
public:
    ~OBBCube();
    void SetVertex(const Vector3(&vertices)[8]);
    void Create(uint32_t& textureHandle);
    void Draw(ID3D12GraphicsCommandList* commandList)override;
private:
    void CreateVertex()override;
    void CreateIndexResource()override;
};

