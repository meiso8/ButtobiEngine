#include"MaterialResource.h"
#include"MakeMatrix.h"
#include"DirectXCommon.h"

MaterialResource::~MaterialResource()
{
    if (materialResource_) {
        materialResource_->Unmap(0, nullptr);
    }
    materialResource_.Reset();
}

void MaterialResource::CreateMaterial(const Vector4& color, uint32_t lightType) {

    //マテリアル用のリソースを作る。
    materialResource_ = DirectXCommon::CreateBufferResource(sizeof(Material));
    //マテリアルにデータを書き込む

    //書き込むためのアドレスを取得
    HRESULT result = materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&material_));
    material_->color = color;
    SetLightType(lightType);
    material_->uvTransform = MakeIdentity4x4();


};

void MaterialResource::SetColor(const Vector4& color) {
    material_->color = color;
}


void MaterialResource::SetMaterial(Material* material) {
    material_ = material;
}

void MaterialResource::SetUV(const Matrix4x4& transform) {
    material_->uvTransform = transform
        ;
}

void MaterialResource::SetLightType(uint32_t lightType)
{
    material_->lightType = lightType;
}
