#pragma once

#include"MeshCommon.h"

class LineMesh :public MeshCommon
{
public:
    ~LineMesh();
    void Create(const TextureFactory::Handle& textureHandle = TextureFactory::WHITE_1X1);
    void PreDraw(ID3D12GraphicsCommandList* commandList, const BlendMode& blendMode = BlendMode::kBlendModeNormal,const CullMode& cullMode = CullMode::kCullModeBack)override;
    void Draw(ID3D12GraphicsCommandList* commandList)override;

    void SetVertexData(const Vector3& start, const Vector3& end);

private:
    void CreateVertex()override;
};

