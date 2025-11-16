#pragma once
#include<d3d12.h>
#include<wrl.h>
#include"TransformationMatrix.h"  
#include"Camera.h"  
#include"MeshCommon.h"
#include"WorldTransform.h"
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
public:
    void Create();
    void Initialize();
    void Update();
    void Draw(Camera& camera, const BlendMode& blendMode = BlendMode::kBlendModeNormal, const CullMode& cullMode = CullMode::kCullModeBack);
    void SetMesh(MeshCommon* mesh) { meshCommon_ = mesh; };
    void SetLightMode(const LightMode& lightMode) { meshCommon_->SetLightMode(lightMode); };
    void SetColor(const Vector4& color) { meshCommon_->SetColor(color); };
    void SetTextureHandle(const Texture::TEXTURE_HANDLE& handle) { meshCommon_->SetTextureHandle(handle); };
private:
    void CreateTransformationMatrix();
};

