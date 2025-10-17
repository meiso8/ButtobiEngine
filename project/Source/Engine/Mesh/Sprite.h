#pragma once  

#include"PSO.h"  
#include"ShaderResourceView.h"  

#include"Transform.h"  
#include"TransformationMatrix.h"  
#include"MaterialResource.h"  
#include"Vector2.h"  
#include"RootSignature.h"  

#include<d3d12.h>
#include"SpriteCommon.h"

class Sprite
{
public:
    void Create(uint32_t textureHandle, const Vector2& position, const Vector2& size,const Vector4& color = { 1.0f,1.0f,1.0f,1.0f });

    void Update();

    static void PreDraw(uint32_t blendMode = BlendMode::kBlendModeNormal);
    void Draw(uint32_t lightType = MaterialResource::LIGHTTYPE::NONE
    );

    void SetColor(const Vector4& color);
    void SetTexture(uint32_t textureHandle) { textureIndex = textureHandle; };

    void SetSize(const Vector2& size) { size_ = size; };
    void SetPosition(const Vector2& position) { position_ = position; }
    void SetRotate(const float& rotate) { rotate_ = rotate; }
    void SetScale(const Vector3& scale) { transform_.scale = scale; };

    Vector2& GetSize() { return size_; }
    Vector3& GetScale() { return transform_.scale; };
    float& GetRotate() { return rotate_; };
    Vector2& GetPosition() { return position_; };

    Material* GetMaterial() { return materialResource_.GetMaterial(); };
    Vector3& GetUVScale() { return uvTransform_.scale; };
    Vector3& GetUVRotate() { return uvTransform_.rotate; };
    Vector3& GetUVTranslate() { return uvTransform_.translate; };
    const Vector4& GetColor() { return materialResource_.GetMaterial()->color; }

    Vector2& GetAnchorPoint() { return anchorPoint_; }
    void SetAnchorPoint(const Vector2& anchorPoint) { anchorPoint_ = anchorPoint; }
    void SetIsFlipX(const bool isFlipX) { isFlipX_ = isFlipX; };
    void SetIsFlipY(const bool isFlipY) { isFlipY_ = isFlipY; };
    bool& GetIsFlipX() { return isFlipX_; };
    bool& GetIsFlipY() { return isFlipY_; };

    void SetTextureLeftTop(const Vector2& leftTop) { textureLeftTop = leftTop; }
    void SetTextureSize(const Vector2& size) { textureSize = size; };
    Vector2& GetTextureLeftTop() { return textureLeftTop; };
    Vector2& GetTextureSize() { return textureSize; };
private:
    void CreateVertex();
    void CreateUVTransformationMatrix();
    void CreateTransformationMatrix();
    void CreateMaterial(const Vector4& color);
    void UpdateUV();
    void AdjustTextureSize();
private:
    uint32_t textureIndex = 0;
    Vector2 anchorPoint_ = { 0.0f,0.0f };
    bool isFlipX_ = false;
    bool isFlipY_ = false;
    Vector2 textureLeftTop = { 0.0f,0.0f };
    Vector2 textureSize = { 100.0f,100.0f };
    static ID3D12GraphicsCommandList* commandList;

    Microsoft::WRL::ComPtr <ID3D12Resource> vertexResource_{};
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    VertexData* vertexData_ = nullptr;


    Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResource_ = nullptr;
    Vector2 position_ = { 0.0f,0.0f };
    float rotate_ = 0.0f;
    Vector2 size_ = { 0.0f,0.0f };

    Transform transform_{};
    Matrix4x4 worldMatrix_{};
    Matrix4x4 worldViewProjectionMatrix_{};
    TransformationMatrix* transformationMatrixData_ = nullptr;

    Transform uvTransform_ = { 0.0f };
    Matrix4x4 uvTransformMatrix_{};

    MaterialResource materialResource_{};


};