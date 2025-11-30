#include "CircleMesh.h"
#include"Circle.h"

#include"DirectXCommon.h"
#include"TransformationMatrix.h"
#include"MakeMatrix.h"
#include"Transform.h"
#include<numbers>
#include"SRVmanager/SrvManager.h"


CircleMesh::~CircleMesh()
{
    Finalize();
}

void CircleMesh::Create(const Texture::TEXTURE_HANDLE& textureHandle)
{
    modelConfig_ = ModelConfig::GetInstance();
    textureHandle_ = Texture::GetHandle(textureHandle);

    CreateVertex();
    CreateIndexResource();

}



void CircleMesh::SetVertex(const Circle& circle)
{
    vertexResource_->Map(0, nullptr,
        reinterpret_cast<void**>(&vertexData_));

    //緯度の方向に分割　-pi/2 ~ pi/2
    const float pi = std::numbers::pi_v<float>;
    const float kLonEvery = 2.0f * pi / float(kSubdivision_);

    //経度の方向に分割 0 ~ 2*pi
    for (uint32_t lonIndex = 0; lonIndex < kSubdivision_; ++lonIndex) {
        float lon = lonIndex * kLonEvery;//現在の経度
        Vector2 uv = { float(lonIndex) / float(kSubdivision_),
            1.0f };
        vertexData_[lonIndex].position.x = std::cos(lon) * circle.radius + circle.center.x;
        vertexData_[lonIndex].position.y = circle.center.y;
        vertexData_[lonIndex].position.z = std::sin(lon) * circle.radius + circle.center.z;
        vertexData_[lonIndex].position.w = 1.0f;
        vertexData_[lonIndex].texcoord = uv;
    }

    //ここが中心点
    vertexData_[kSubdivision_].position.x = circle.center.x;
    vertexData_[kSubdivision_].position.y = circle.center.y;
    vertexData_[kSubdivision_].position.z = circle.center.z;
    vertexData_[kSubdivision_].position.w = 1.0f;
    vertexData_[kSubdivision_].texcoord = { 0.0f,0.0f };
    for (uint32_t index = 0; index < kSubdivision_ + 1; ++index) {

        vertexData_[index].normal = {
            vertexData_[index].position.x ,
             vertexData_[index].position.y,
              vertexData_[index].position.z
        };
    }


    vertexResource_->Unmap(0, nullptr);
}


void CircleMesh::Draw(ID3D12GraphicsCommandList* commandList) {


    //頂点バッファビューを設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
    //IBVを設定new
    commandList->IASetIndexBuffer(&indexBufferView_);//IBVを設定

    //SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    SrvManager::SetGraphicsRootDescriptorTable(2, textureHandle_);
    //LightのCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(3, modelConfig_->directionalLightResource->GetGPUVirtualAddress());

    //描画!（DrawCall/ドローコール）6個のインデックスを使用し1つのインスタンスを描画。その他は当面0で良い。
    commandList->DrawIndexedInstanced(3 * kSubdivision_, 1, 0, 0, 0);
}

void CircleMesh::CreateVertex() {

    //VertexResourceとVertexBufferViewを用意 矩形を表現するための三角形を二つ(頂点4つ)
    vertexResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * (kSubdivision_ + 1));

    //頂点バッファビューを作成する
    //リソースの先頭アドレスから使う
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    //使用するリソースのサイズ頂点4つ分のサイズ
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * (kSubdivision_ + 1);
    //1頂点あたりのサイズ
    vertexBufferView_.StrideInBytes = sizeof(VertexData);


    Circle circle{ .center = {0.0f,0.0f,0.0f },.radius = 4.0f };
    SetVertex(circle);

}

void CircleMesh::CreateIndexResource() {

#pragma region//IndexResourceを作成
    indexResource_ = DirectXCommon::CreateBufferResource(sizeof(uint32_t) * kSubdivision_ * 3);
    //Viewを作成する IndexBufferView(IBV)

    //リソースの先頭アドレスから使う
    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    //使用するリソースのサイズはインデックス6つ分のサイズ
    indexBufferView_.SizeInBytes = sizeof(uint32_t) * kSubdivision_ * 3;
    //インデックスはuint32_tとする
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
#pragma endregion

#pragma region//IndexResourceにデータを書き込む
    //インデックスリーソースにデータを書き込む
    indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

    uint32_t index = 0;

    for (uint32_t i = 0; i < kSubdivision_ * 3; i += 3) {


        indexData_[i] = index + 1;
        indexData_[i + 1] = index;

        if (index == kSubdivision_ - 1) {
            indexData_[i] = 0;
            indexData_[i + 1] = index;

        }

        indexData_[i + 2] = kSubdivision_;

        index++;
    }

    indexResource_->Unmap(0, nullptr);
#pragma endregion
}