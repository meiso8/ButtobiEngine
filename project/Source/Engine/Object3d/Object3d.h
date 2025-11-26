#pragma once
#include<d3d12.h>
#include<wrl.h>
#include"TransformationMatrix.h"  
#include"Camera.h"  
#include"MeshCommon.h"
#include"WorldTransform.h"
#include"MaterialResource.h"  

#include<memory>
enum LightMode;
class Object3d
{

public:
    WorldTransform worldTransform_;
private:
    //位置情報
    Microsoft::WRL::ComPtr <ID3D12Resource> transformationMatrixResource_ = nullptr;
    TransformationMatrixFor3D* transformationMatrixData_ = nullptr;
    //メッシュ情報
    MeshCommon* meshCommon_ = nullptr;
    //コマンドリスト 借り物
    static  ID3D12GraphicsCommandList* commandList_;

    //マテリアルリソース
    std::unique_ptr<MaterialResource> materialResource_ = nullptr;
    Transform uvTransform_ = { 0.0f };
    Matrix4x4 uvTransformMatrix_{};

public:
    void UpdateUV();

    Material& GetMaterial() { return *materialResource_->GetMaterial(); };

    Vector3& GetUVScale() { return uvTransform_.scale; };
    Vector3& GetUVRotate() { return uvTransform_.rotate; };
    Vector3& GetUVTranslate() { return uvTransform_.translate; };

    Transform& GetUVTransform() { return uvTransform_;  }

    int32_t& GetLightMode() { return materialResource_->GetMaterial()->lightMode; };
    Vector4& GetColor() { return materialResource_->GetMaterial()->color; };

    void SetUVTransform(const Transform& transform) {  uvTransform_ = transform; UpdateUV(); };
    void SetMesh(MeshCommon* mesh) { meshCommon_ = mesh; };
    void SetTextureHandle(const Texture::TEXTURE_HANDLE& handle) { meshCommon_->SetTextureHandle(handle); };
    void SetColor(const Vector4& color) { materialResource_->SetColor(color); }
    void SetLightMode(const LightMode& lightMode) { materialResource_->SetLightMode(lightMode); }

    void Create();
    void Initialize();
    void Update();
    void Draw(Camera& camera, const BlendMode& blendMode = BlendMode::kBlendModeNormal, const CullMode& cullMode = CullMode::kCullModeBack);
    
private:
    void CreateUV();
    void CreateTransformationMatrix();
    void CreateMaterial(const Vector4& color = { 1.0f,1.0f,1.0f,1.0f }, const uint32_t& lightType = 0);
};

