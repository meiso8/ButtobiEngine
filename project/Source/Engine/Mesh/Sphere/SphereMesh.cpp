#include "SphereMesh.h"
#include"DirectXCommon.h"
#include"TransformationMatrix.h"
#include"MakeMatrix.h"
#include"Transform.h"
#include<numbers>
#include"SRVmanager/SrvManager.h"

SphereMesh::~SphereMesh()
{
    Finalize();
}

void SphereMesh::Create(const Texture::TEXTURE_HANDLE& textureHandle)
{
  modelConfig_ = ModelConfig::GetInstance();
  textureHandle_ = Texture::GetHandle(textureHandle);

    CreateVertex();
    //CreateIndexResource();
}



void SphereMesh::SetVertex(const Sphere& spehre)
{
    const float pi = std::numbers::pi_v<float>;
    const float kLonEvery = 2.0f * pi / float(kSubdivision_);
    const float kLatEvery = pi / float(kSubdivision_);

    for (uint32_t latIndex = 0; latIndex < kSubdivision_; ++latIndex) {
        float lat = -pi / 2.0f + kLatEvery * latIndex;//現在の緯度
        //経度の方向に分割 0 ~ 2*pi
        for (uint32_t lonIndex = 0; lonIndex < kSubdivision_; ++lonIndex) {
            //書き込む最初の場所
            uint32_t startIndex = (latIndex * kSubdivision_ + lonIndex) * 6;
            float lon = lonIndex * kLonEvery;//現在の経度

            Vector2 uv = { float(lonIndex) / float(kSubdivision_),
                1.0f - float(latIndex) / float(kSubdivision_) };

            //a   
            vertexData_[startIndex].position.x = std::cos(lat) * std::cos(lon);
            vertexData_[startIndex].position.y = std::sin(lat);
            vertexData_[startIndex].position.z = std::cos(lat) * std::sin(lon);
            vertexData_[startIndex].position.w = 1.0f;
            vertexData_[startIndex].texcoord = uv;

            //b
            vertexData_[startIndex + 1].position.x = std::cos(lat + kLatEvery) * std::cos(lon);
            vertexData_[startIndex + 1].position.y = std::sin(lat + kLatEvery);
            vertexData_[startIndex + 1].position.z = std::cos(lat + kLatEvery) * std::sin(lon);
            vertexData_[startIndex + 1].position.w = 1.0f;
            vertexData_[startIndex + 1].texcoord = { uv.x,
               uv.y - 1.0f / float(kSubdivision_) };

            //c
            vertexData_[startIndex + 2].position.x = std::cos(lat) * std::cos(lon + kLonEvery);
            vertexData_[startIndex + 2].position.y = std::sin(lat);
            vertexData_[startIndex + 2].position.z = std::cos(lat) * std::sin(lon + kLonEvery);
            vertexData_[startIndex + 2].position.w = 1.0f;
            vertexData_[startIndex + 2].texcoord = { uv.x + 1.0f / float(kSubdivision_),
                 uv.y };

            //c
            vertexData_[startIndex + 3] = vertexData_[startIndex + 2];

            //b
            vertexData_[startIndex + 4] = vertexData_[startIndex + 1];

            //d
            vertexData_[startIndex + 5].position.x = std::cos(lat + kLatEvery) * std::cos(lon + kLonEvery);
            vertexData_[startIndex + 5].position.y = std::sin(lat + kLatEvery);
            vertexData_[startIndex + 5].position.z = std::cos(lat + kLatEvery) * std::sin(lon + kLonEvery);
            vertexData_[startIndex + 5].position.w = 1.0f;
            vertexData_[startIndex + 5].texcoord = { uv.x + 1.0f / float(kSubdivision_),
                uv.y - 1.0f / float(kSubdivision_) };

            for (int i = 0; i < 6; ++i) {
                vertexData_[startIndex + i].position.x = (vertexData_[startIndex + i].position.x+spehre.center.x)* spehre.radius;
                vertexData_[startIndex + i].position.y = (vertexData_[startIndex + i].position.y+spehre.center.y)* spehre.radius;
                vertexData_[startIndex + i].position.z = (vertexData_[startIndex + i].position.z+spehre.center.z)* spehre.radius;
                vertexData_[startIndex + i].normal = { vertexData_[startIndex + i].position.x , vertexData_[startIndex + i].position.y, vertexData_[startIndex + i].position.z };
            }


        }

    }



}


void SphereMesh::Draw(ID3D12GraphicsCommandList* commandList) {


    //頂点バッファビューを設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
 //SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    SrvManager::SetGraphicsRootDescriptorTable(2, textureHandle_);
    //LightのCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(3, modelConfig_->directionalLightResource->GetGPUVirtualAddress());
  //描画!(DrawCall/ドローコール)。
    commandList->DrawInstanced(6 * kSubdivision_ * kSubdivision_, 1, 0, 0);

}

void SphereMesh::CreateVertex() {

    //VertexResourceとVertexBufferViewを用意 矩形を表現するための三角形を二つ(頂点4つ)
    vertexResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * 6 * kSubdivision_ * kSubdivision_);

    //頂点バッファビューを作成する
    //リソースの先頭アドレスから使う
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    //使用するリソースのサイズ頂点4つ分のサイズ
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6 * kSubdivision_ * kSubdivision_;
    //1頂点あたりのサイズ
    vertexBufferView_.StrideInBytes = sizeof(VertexData);

#pragma region//Resourceにデータを書き込む
    //頂点リソースにデータを書き込む
    //書き込むためのアドレスを取得
    vertexResource_->Map(0, nullptr,
        reinterpret_cast<void**>(&vertexData_));

    Sphere sphere{ .center = {0.0f,0.0f,0.0f },.radius = 0.5f };

    SetVertex(sphere);

    //緯度の方向に分割　-pi/2 ~ pi/2

    vertexResource_->Unmap(0, nullptr);
}

