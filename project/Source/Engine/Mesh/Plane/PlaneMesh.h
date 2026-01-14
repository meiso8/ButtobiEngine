#pragma once
#include"MeshCommon.h"

class PlaneMesh :public MeshCommon
{
public:
    void Create(const TextureFactory::Handle& textureHandle = TextureFactory::UV_CHECKER);
void Draw(ID3D12GraphicsCommandList* commandList)override;

    void ResetSize(const Vector2& size);

    ~PlaneMesh();
private:
    void CreateVertex()override;
    void CreateIndexResource()override;
private:
    Vector2 size_ = { 1.0f,1.0f };

};

