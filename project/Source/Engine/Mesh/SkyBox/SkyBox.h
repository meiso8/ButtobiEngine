#pragma once
#include"MeshCommon.h"
struct AABB;

class SkyBox :public MeshCommon
{
public:
    ~SkyBox();
    void SetMinMax(const AABB& aabb);
    void SetVertex(const Vector3(&vertices)[8]);
    //デフォルト値を設定
    void Create(const TextureFactory::Handle& textureHandle = TextureFactory::UV_CHECKER);
    void Draw(ID3D12GraphicsCommandList* commandList)override;
protected:
    void CreateVertex()override;
    void CreateIndexResource()override;
};

