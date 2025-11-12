#pragma once
#include"MeshCommon.h"
struct AABB;

class Cube :public MeshCommon
{
public:
    ~Cube();
    void SetMinMax(const AABB& aabb);
    void SetVertex(const Vector3(&vertices)[8]);
    void Create(const Texture::TEXTURE_HANDLE& textureHandle);
    void Draw(ID3D12GraphicsCommandList* commandList)override;
protected:
    void CreateVertex()override;
    void CreateIndexResource()override;
};

