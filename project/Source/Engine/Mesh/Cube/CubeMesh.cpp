#include "CubeMesh.h"
#include"DirectXCommon.h"
#include"MyEngine.h"
#include"AABB.h"

CubeMesh::~CubeMesh()
{
    Finalize();
}

void CubeMesh::Create(const Texture::TEXTURE_HANDLE& textureHandle) {

    modelConfig_ = ModelConfig::GetInstance();
    textureHandle_ = Texture::GetHandle(textureHandle);
    CreateVertex();
    AABB aabb = { { -0.5f,-0.5f,-0.5f },{ 0.5f,0.5f,0.5f } };
    SetMinMax(aabb);
    CreateIndexResource();

    CreatePointLightData();

};

void CubeMesh::CreateVertex()
{
    vertexResource_ = DirectXCommon::CreateBufferResource(sizeof(VertexData) * 20);

    //頂点バッファビューを作成する
    vertexBufferView_.BufferLocation = vertexResource_->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(VertexData) * 20;
    vertexBufferView_.StrideInBytes = sizeof(VertexData);
}

void CubeMesh::CreateIndexResource() {

#pragma region//IndexResourceを作成
    indexResource_ = DirectXCommon::CreateBufferResource(sizeof(uint32_t) * 36);
    //Viewを作成する IndexBufferView(IBV)

    //リソースの先頭アドレスから使う
    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = sizeof(uint32_t) * 36;
    //インデックスはuint32_tとする
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
#pragma endregion

#pragma region//IndexResourceにデータを書き込む
    //インデックスリーソースにデータを書き込む
    indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

    //頂点数を削減
// 上面
    indexData_[0] = 1; indexData_[1] = 3; indexData_[2] = 0;
    indexData_[3] = 0; indexData_[4] = 3; indexData_[5] = 2;

    // tei
    indexData_[6] = 7; indexData_[7] = 5; indexData_[8] = 6;
    indexData_[9] = 6; indexData_[10] = 5; indexData_[11] = 4;

    // front face
    indexData_[12] = 5; indexData_[13] = 8; indexData_[14] = 4;
    indexData_[15] = 4; indexData_[16] = 8; indexData_[17] = 9;

    // Right face
    indexData_[18] = 11; indexData_[19] = 10; indexData_[20] = 12;
    indexData_[21] = 12; indexData_[22] = 10; indexData_[23] = 2;

    // back face
    indexData_[24] = 15; indexData_[25] = 13; indexData_[26] = 16;
    indexData_[27] = 16; indexData_[28] = 13; indexData_[29] = 14;

    // left face
    indexData_[30] = 18; indexData_[31] = 3; indexData_[32] = 19;
    indexData_[33] = 19; indexData_[34] = 3; indexData_[35] = 17;

    indexResource_->Unmap(0, nullptr);

#pragma endregion
}

void CubeMesh::SetVertex(const Vector3(&vertices)[8]) {

    vertexResource_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));

    {
        //上面手前右
        vertexData_[0].position = {
            vertices[4].x,
            vertices[4].y,
            vertices[4].z,1.0f };
        vertexData_[0].texcoord = { 1.0f,1.0f };
        vertexData_[0].normal = vertices[4];//法線
        //上面手前左
        vertexData_[1].position = {
            vertices[5].x,
            vertices[5].y,
            vertices[5].z,1.0f };
        vertexData_[1].texcoord = { 0.0f,1.0f };
        vertexData_[1].normal = vertices[5];//法線

        //上面奥右
        vertexData_[2].position = {
            vertices[0].x,
            vertices[0].y,
            vertices[0].z,1.0f };
        vertexData_[2].texcoord = { 1.0f,0.0f };
        vertexData_[2].normal = vertices[0];//法線

        //上面奥左
        vertexData_[3].position = {
            vertices[1].x,
            vertices[1].y,
            vertices[1].z,1.0f };
        vertexData_[3].texcoord = { 0.0f,0.0f };
        vertexData_[3].normal = vertices[1];//法線

    }

    {
        //底面手前右
        vertexData_[4].position = {
            vertices[6].x,
            vertices[6].y,
            vertices[6].z,1.0f };
        vertexData_[4].texcoord = { 1.0f,1.0f };
        vertexData_[4].normal = vertices[6];//法線
        //底面手前左
        vertexData_[5].position = {
            vertices[7].x,
            vertices[7].y,
            vertices[7].z,1.0f };
        vertexData_[5].texcoord = { 0.0f,1.0f };
        vertexData_[5].normal = vertices[7];//法線

        //底面奥右
        vertexData_[6].position = {
            vertices[2].x,
            vertices[2].y,
            vertices[2].z,1.0f };
        vertexData_[6].texcoord = { 1.0f,0.0f };
        vertexData_[6].normal = vertices[2];//法線

        //底面奥左
        vertexData_[7].position = {
            vertices[3].x,
            vertices[3].y,
            vertices[3].z,
            1.0f };
        vertexData_[7].texcoord = { 0.0f,0.0f };
        vertexData_[7].normal = vertices[3];//法線

    }


    {

        //上面手前左
        vertexData_[8].position = vertexData_[1].position;
        vertexData_[8].texcoord = { 0.0f,0.0f };
        vertexData_[8].normal = vertexData_[1].normal;//法線

        //上面手前右
        vertexData_[9].position = vertexData_[0].position;
        vertexData_[9].texcoord = { 1.0f,0.0f };
        vertexData_[9].normal = vertexData_[0].normal;//法線

    }

    {
        //上面手前右
        vertexData_[10].position = vertexData_[0].position;
        vertexData_[10].texcoord = { 0.0f,0.0f };
        vertexData_[10].normal = vertexData_[0].normal;//法線

        //底面手前右
        vertexData_[11].position = vertexData_[4].position;
        vertexData_[11].texcoord = { 0.0f,1.0f };
        vertexData_[11].normal = vertexData_[4].normal;//法線

        //底面奥右
        vertexData_[12].position = vertexData_[6].position;
        vertexData_[12].texcoord = { 1.0f,1.0f };
        vertexData_[12].normal = vertexData_[6].normal;//法線

    }

    {
        //上面奥右
        vertexData_[13].position = vertexData_[2].position;
        vertexData_[13].texcoord = { 0.0f,0.0f };
        vertexData_[13].normal = vertexData_[2].normal;//法線

        //上面奥左 かぶり
        vertexData_[14].position = vertexData_[3].position;
        vertexData_[14].texcoord = { 1.0f,0.0f };
        vertexData_[14].normal = vertexData_[3].normal;//法線

        //底面奥右
        vertexData_[15].position = vertexData_[6].position;
        vertexData_[15].texcoord = { 0.0f,1.0f };
        vertexData_[15].normal = vertexData_[6].normal;//法線

        //底面奥左
        vertexData_[16].position = vertexData_[7].position;
        vertexData_[16].texcoord = { 1.0f,1.0f };
        vertexData_[16].normal = vertexData_[7].normal;//法線
    }

    {

        //上面手前左
        vertexData_[17].position = vertexData_[1].position;
        vertexData_[17].texcoord = { 1.0f,0.0f };
        vertexData_[17].normal = vertexData_[1].normal;//法線

        //底面奥左
        vertexData_[18].position = vertexData_[7].position;
        vertexData_[18].texcoord = { 0.0f,1.0f };
        vertexData_[18].normal = vertexData_[7].normal;//法線

        //底面手前左
        vertexData_[19].position = vertexData_[5].position;
        vertexData_[19].texcoord = { 1.0f,1.0f };
        vertexData_[19].normal = vertexData_[5].normal;//法線

    }

    vertexResource_->Unmap(0, nullptr);
}

void CubeMesh::Draw(ID3D12GraphicsCommandList* commandList)
{

    //頂点バッファビューを設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
    ////IBVを設定new
    commandList->IASetIndexBuffer(&indexBufferView_);//IBVを設定
 
    //SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    SrvManager::SetGraphicsRootDescriptorTable(2, textureHandle_);
    //LightのCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(3, modelConfig_->directionalLightResource->GetGPUVirtualAddress());

    //expansionのCBufferの場所を設定
    commandList->SetGraphicsRootConstantBufferView(7, pointLightResource_->GetGPUVirtualAddress());

    //描画!（DrawCall/ドローコール）6個のインデックスを使用し1つのインスタンスを描画。その他は当面0で良い。
    commandList->DrawIndexedInstanced(36, 1, 0, 0, 0);
}


void CubeMesh::SetMinMax(const AABB& aabb) {

    Vector3 vertexData[8] = {};

    vertexData[0] = { aabb.max.x,aabb.max.y,aabb.max.z };  //上面奥右
    vertexData[1] = { aabb.min.x,aabb.max.y,aabb.max.z };    //上面奥左   
    vertexData[2] = { aabb.max.x,aabb.min.y,aabb.max.z };    //底面奥右     
    vertexData[3] = { aabb.min.x,aabb.min.y,aabb.max.z };  //底面奥左
    vertexData[4] = { aabb.max.x,aabb.max.y,aabb.min.z };  //上面手前右
    vertexData[5] = { aabb.min.x,aabb.max.y,aabb.min.z };     //上面手前左
    vertexData[6] = { aabb.max.x,aabb.min.y,aabb.min.z };    //底面手前右
    vertexData[7] = { aabb.min.x,aabb.min.y,aabb.min.z };        //底面手前左

    SetVertex(vertexData);
}


