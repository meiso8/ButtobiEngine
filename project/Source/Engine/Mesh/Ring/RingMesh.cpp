#include "RingMesh.h"
#include"Circle.h"

#include"DirectXCommon.h"
#include<numbers>
#include"SRVmanager/SrvManager.h"

#include"PSO.h"

RingMesh::~RingMesh()
{
    Finalize();
}

void RingMesh::Create(const TextureFactory::Handle& textureHandle)
{
    textureHandle_ = Texture::GetHandle(textureHandle);

    CreateVertex();
    CreateIndexResource();

}



void RingMesh::SetVertex()
{
    vertexResource_->Map(0, nullptr,
        reinterpret_cast<void**>(&vertexData_));

    //緯度の方向に分割　-pi/2 ~ pi/2
    const float pi = std::numbers::pi_v<float>;
    const float radianPreDivide = 2.0f * pi / float(kRingDivide_);

    for (int index = 0; index < int(kRingDivide_); ++index) {
        float sin = std::sin(index * radianPreDivide);
        float cos = std::cos(index * radianPreDivide);
        float sinNext = std::sin((index + 1) * radianPreDivide);
        float cosNext = std::cos((index + 1) * radianPreDivide);
        float u = float(index) / float(kRingDivide_);
        float uNext = float(index + 1) / float(kRingDivide_);

        int indexes = 4 * index;

        vertexData_[indexes].position =
        { -sin * kOuterRadius_, cos * kOuterRadius_, 0.0f, 1.0f };
        vertexData_[indexes].texcoord = { u,0.0f };

        indexes++;

        vertexData_[indexes].position =
        { -sinNext * kOuterRadius_, cosNext * kOuterRadius_, 0.0f, 1.0f };
        vertexData_[indexes].texcoord = { uNext,0.0f };

        indexes++;

        vertexData_[indexes].position =
        { -sin * kInnerRadius_, cos * kInnerRadius_, 0.0f, 1.0f };
        vertexData_[indexes].texcoord = { u, 1.0f };

        indexes++;

        vertexData_[indexes].position =
        { -sinNext * kInnerRadius_, cosNext * kInnerRadius_, 0.0f, 1.0f };
        vertexData_[indexes].texcoord = { uNext, 1.0f };
    }

    for (int i = 0; i < int(kRingDivide_ * 4); ++i) {
        const auto& pos = vertexData_[i].position;
        vertexData_[i].normal = { pos.x,pos.y,pos.z };
    }

    vertexResource_->Unmap(0, nullptr);
}


void RingMesh::Draw(ID3D12GraphicsCommandList* commandList) {


    //頂点バッファビューを設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
    //IBVを設定new
    commandList->IASetIndexBuffer(&indexBufferView_);//IBVを設定

    //SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    SrvManager::SetGraphicsRootDescriptorTable(2, textureHandle_);

    //描画!（DrawCall/ドローコール）6個のインデックスを使用し1つのインスタンスを描画。その他は当面0で良い。
    commandList->DrawIndexedInstanced(kRingDivide_ * 6, 1, 0, 0, 0);
}

void RingMesh::CreateVertex() {

    //VertexResourceとVertexBufferViewを用意 矩形を表現するための三角形を二つ(頂点4つ)
    vertexResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * (kRingDivide_ * 4 + 1));

    //頂点バッファビューを作成する
    //リソースの先頭アドレスから使う
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    //使用するリソースのサイズ頂点4つ分のサイズ
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * (kRingDivide_ * 4 + 1);
    //1頂点あたりのサイズ
    vertexBufferView_.StrideInBytes = sizeof(VertexData);
    SetVertex();
}

void RingMesh::CreateIndexResource() {

#pragma region//IndexResourceを作成
    indexResource_ = DirectXCommon::CreateBufferResource(sizeof(uint32_t) * kRingDivide_*6);
    //Viewを作成する IndexBufferView(IBV)

    //リソースの先頭アドレスから使う
    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    //使用するリソースのサイズはインデックス6つ分のサイズ
    indexBufferView_.SizeInBytes = sizeof(uint32_t) * kRingDivide_ * 6;
    //インデックスはuint32_tとする
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
#pragma endregion

#pragma region//IndexResourceにデータを書き込む
    //インデックスリーソースにデータを書き込む
    indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

    uint32_t indexCount = 0;

    for (uint32_t i = 0; i < kRingDivide_; ++i) {
        int vertexIndex = i * 4;
        indexData_[indexCount++] = vertexIndex;
        indexData_[indexCount++] = vertexIndex + 1;
        indexData_[indexCount++] = vertexIndex + 2;

        indexData_[indexCount++] = vertexIndex + 2;
        indexData_[indexCount++] = vertexIndex + 1;
        indexData_[indexCount++] = vertexIndex + 3;
    }

    indexResource_->Unmap(0, nullptr);
#pragma endregion
}