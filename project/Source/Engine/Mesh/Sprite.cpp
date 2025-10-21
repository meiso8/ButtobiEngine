#include "Sprite.h"
#include"DirectXCommon.h"
#include"TransformationMatrix.h"
#include"MakeMatrix.h"
#include"MyEngine.h"
#include"TextureManager.h"
#include"Camera/SpriteCamera.h"  
#include"ImGuiClass.h"

ID3D12GraphicsCommandList* Sprite::commandList = nullptr;

void Sprite::Create(uint32_t textureHandle, const Vector2& position, const Vector2& size, const Vector4& color)
{
    commandList = DirectXCommon::GetCommandList();
    position_ = position;
    textureIndex = textureHandle;

    CreateMaterial(color);
    CreateVertex();
    CreateTransformationMatrix();
    CreateUVTransformationMatrix();
    AdjustTextureSize();

    size_ = size;
}

void Sprite::Update()
{
    float left = 0.0f - anchorPoint_.x;
    float right = 1.0f - anchorPoint_.x;
    float top = 0.0f - anchorPoint_.y;
    float bottom = 1.0f - anchorPoint_.y;

    if (isFlipX_) {
        left = -left;
        right = -right;
    }

    if (isFlipY_) {
        top = -top;
        bottom = -bottom;
    }



    vertexData_[0].position = { left,bottom,0.0f,1.0f };//左下
    vertexData_[1].position = { left,top,0.0f,1.0f };//左上
    vertexData_[2].position = { right,bottom,0.0f,1.0f };//右下
    vertexData_[3].position = { right,top,0.0f,1.0f };//右上

    const DirectX::TexMetadata& metadata = TextureManager::GetMetaData(textureIndex);
    float tex_left = textureLeftTop.x / metadata.width;
    float tex_right = (textureLeftTop.x + textureSize.x) / metadata.width;
    float tex_top = textureLeftTop.y / metadata.height;
    float tex_bottom = (textureLeftTop.y + textureSize.y) / metadata.height;

    vertexData_[0].texcoord = {tex_left,tex_bottom};
    vertexData_[1].texcoord = {tex_left,tex_top };
    vertexData_[2].texcoord = {tex_right,tex_bottom};
    vertexData_[3].texcoord = {tex_right,tex_top };

    UpdateUV();
}

void Sprite::SetColor(const Vector4& color) {
    materialResource_.SetColor(color);
}


void Sprite::PreDraw(uint32_t blendMode) {
    SpriteCommon::PreDraw(commandList);
    commandList->SetPipelineState(MyEngine::GetPSO()->GetGraphicsPipelineStateSprite(blendMode).Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Sprite::Draw(uint32_t lightType
) {

    materialResource_.SetLightType(lightType);
    transform_.scale = { size_.x,size_.y,1.0f };
    transform_.rotate = { 0.0f,0.0f,rotate_ };
    transform_.translate = { position_.x,position_.y,0.0f };

    worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
    worldViewProjectionMatrix_ = Multiply(worldMatrix_, SpriteCamera::GetViewProjectionMatrix());
    *transformationMatrixData_ = { worldViewProjectionMatrix_,worldMatrix_ };

    //頂点バッファビューを設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
    SpriteCommon::SetIndexBuffer(commandList);
    //マテリアルCBufferの場所を設定　/*RotParameter配列の0番目 0->register(b4)1->register(b0)2->register(b4)*/
    commandList->SetGraphicsRootConstantBufferView(0, materialResource_.GetMaterialResource()->GetGPUVirtualAddress());
    //TransformationMatrixCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
    //SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    commandList->SetGraphicsRootDescriptorTable(2, TextureManager::GetSrvHandleGPU(textureIndex));

    SpriteCommon::DrawCall(commandList);

};

// ===================================//Private関数//===================================


void Sprite::CreateVertex()
{
    //VertexResourceとVertexBufferViewを用意 矩形を表現するための三角形を二つ(頂点4つ)
    vertexResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * 4);

    //頂点バッファビューを作成する
    //リソースの先頭アドレスから使う
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    //使用するリソースのサイズ頂点4つ分のサイズ
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
    //1頂点あたりのサイズ
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

#pragma region //Sprite用の頂点データの設定

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
    //1枚目の三角形 四頂点でスプライト描画が完成

    vertexData_[0].position = { 0.0f,1.0f,0.0f,1.0f };//左下
    vertexData_[1].position = { 0.0f,0.0f,0.0f,1.0f };//左上
    vertexData_[2].position = { 1.0f,1.0f,0.0f,1.0f };//右下
    vertexData_[3].position = { 1.0f,0.0f,0.0f,1.0f };//右上


    vertexData_[0].texcoord = { 0.0f,1.0f };
    vertexData_[0].normal = { 0.0f,0.0f,-1.0f };//法線

    vertexData_[1].texcoord = { 0.0f,0.0f };
    vertexData_[1].normal = { 0.0f,0.0f,-1.0f };

    vertexData_[2].texcoord = { 1.0f,1.0f };
    vertexData_[2].normal = { 0.0f,0.0f,-1.0f };

    vertexData_[3].texcoord = { 1.0f,0.0f };
    vertexData_[3].normal = { 0.0f,0.0f,-1.0f };

    vertexResource_->Unmap(0, nullptr);

#pragma endregion

}

void Sprite::CreateUVTransformationMatrix()
{
    uvTransform_ = {
     {1.0f,1.0f,1.0f},
     {0.0f,0.0f,0.0f},
     {0.0f,0.0f,0.0f},
    };

    uvTransformMatrix_ = MakeIdentity4x4();
}

void Sprite::CreateTransformationMatrix() {

    //Matrix4x4　1つ分のサイズを用意
    transformationMatrixResource_ = DirectXCommon::CreateBufferResource(sizeof(TransformationMatrix));
    //データを書き込む
    //書き込むためのアドレスを取得
    transformationMatrixResource_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixData_));

    transform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f} ,{ position_.x,position_.y,0.0f } };
    worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

    transformationMatrixResource_->Unmap(0, nullptr);

}

void Sprite::CreateMaterial(const Vector4& color) {

    //マテリアルリソースを作成 //ライトなし
    materialResource_.CreateMaterial(color, MaterialResource::LIGHTTYPE::NONE);

}

void Sprite::UpdateUV() {
    uvTransformMatrix_ = MakeAffineMatrix(uvTransform_.scale, uvTransform_.rotate, uvTransform_.translate);
    materialResource_.SetUV(uvTransformMatrix_);
}

void Sprite::AdjustTextureSize()
{
    const DirectX::TexMetadata& metadata = TextureManager::GetMetaData(textureIndex);
    textureSize.x = static_cast<float>(metadata.width);
    textureSize.y = static_cast<float>(metadata.height);
    size_ = textureSize;
}
