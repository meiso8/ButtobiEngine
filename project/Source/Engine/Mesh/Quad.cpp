#include "Quad.h"

#include"DirectXCommon.h"
#include"MyEngine.h"
#include"TextureManager.h"
#include"MakeMatrix.h"

QuadMesh::~QuadMesh() {
    Finalize();
}

void QuadMesh::Create(uint32_t textureHandle)
{
    commandList_ = DirectXCommon::GetCommandList();
    modelConfig_ = ModelConfig::GetInstance();
    textureHandle_ = textureHandle;

    CreateVertex();
    CreateIndexResource();
    CreateTransformationMatrix();
    CreateUVTransformationMatrix();
    CreateMaterial();
    CreateWaveData();
    CreateBalloonData();

}

void QuadMesh::CreateVertex() {

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
    ResetSize({ 1.0f,1.0f });


    vertexData_[0].texcoord = { 0.0f, 1.0f };//左下
    vertexData_[1].texcoord = { 0.0f,0.0f };//左上
    vertexData_[2].texcoord = { 1.0f,0.0f };//右下
    vertexData_[3].texcoord = { 1.0f,1.0f };//右上

    vertexData_[0].normal = { 0.0f,0.0f,1.0f };//法線
    vertexData_[1].normal = { 0.0f,0.0f,1.0f };
    vertexData_[2].normal = { 0.0f,0.0f,1.0f };
    vertexData_[3].normal = { 0.0f,0.0f,1.0f };

    vertexResource_->Unmap(0, nullptr);
#pragma endregion


}

void QuadMesh::CreateUVTransformationMatrix()
{
    uvTransform_ = {
     {1.0f,1.0f,1.0f},
     {0.0f,0.0f,0.0f},
     {0.0f,0.0f,0.0f},
    };

    uvTransformMatrix_ = MakeIdentity4x4();
}


void QuadMesh::UpdateUV() {
    uvTransformMatrix_ = MakeAffineMatrix(uvTransform_.scale, uvTransform_.rotate, uvTransform_.translate);
    materialResource_->SetUV(uvTransformMatrix_);
}

void QuadMesh::ResetSize(const Vector2& size) {
    size_ = size;
    Vector2 halfSize = size_ * 0.5f;
    vertexData_[0].position = { -halfSize.x,-halfSize.y,0.0f,1.0f };//左下
    vertexData_[1].position = { -halfSize.x,halfSize.y,0.0f,1.0f };//左上
    vertexData_[2].position = { halfSize.x,halfSize.y,0.0f,1.0f };//右上
    vertexData_[3].position = { halfSize.x,-halfSize.y,0.0f,1.0f };//右下

};

void QuadMesh::PreDraw(const BlendMode& type ,const CullMode& cullMode) {
    commandList_->SetGraphicsRootSignature(modelConfig_->rootSignature->GetRootSignature(0));
    commandList_->SetPipelineState(MyEngine::GetPSO()->GetGraphicsPipelineState(type, cullMode).Get());//PSOを設定
    //形状を設定。PSOに設定している物とはまた別。同じものを設定すると考えておけばよい。
    commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void QuadMesh::Draw(Camera& camera, const Matrix4x4& worldMatrix, const uint32_t lightType)
{
    materialResource_->SetLightType(lightType);

    worldViewProjectionMatrix_ = Multiply(worldMatrix, camera.GetViewProjectionMatrix());

    *transformationMatrixData_ = { worldViewProjectionMatrix_,worldMatrix };



    //頂点バッファビューを設定
    commandList_->IASetVertexBuffers(0, 1, &vertexBufferView_);//VBVを設定
    //IBVを設定new
    commandList_->IASetIndexBuffer(&indexBufferView_);//IBVを設定
    //マテリアルCBufferの場所を設定　/*RotParameter配列の0番目 0->register(b4)1->register(b0)2->register(b4)*/
    commandList_->SetGraphicsRootConstantBufferView(0, materialResource_->GetMaterialResource()->GetGPUVirtualAddress());
    //TransformationMatrixCBufferの場所を設定
    commandList_->SetGraphicsRootConstantBufferView(1, transformationMatrixResource_->GetGPUVirtualAddress());
    //SRVのDescriptorTableの先頭を設定。2はrootParameter[2]である。
    commandList_->SetGraphicsRootDescriptorTable(2, TextureManager::GetSrvHandleGPU(textureHandle_));
    //LightのCBufferの場所を設定
    commandList_->SetGraphicsRootConstantBufferView(3, modelConfig_->directionalLightResource->GetGPUVirtualAddress());
    //timeのSRVの場所を設定
    commandList_->SetGraphicsRootShaderResourceView(4, waveResource_->GetGPUVirtualAddress());
    //expansionのCBufferの場所を設定
    commandList_->SetGraphicsRootConstantBufferView(5, expansionResource_->GetGPUVirtualAddress());
    //cameraのCBufferの場所を設定
    commandList_->SetGraphicsRootConstantBufferView(6, camera.GetResource()->GetGPUVirtualAddress());

    //描画!（DrawCall/ドローコール）6個のインデックスを使用し1つのインスタンスを描画。その他は当面0で良い。
    commandList_->DrawIndexedInstanced(6, 1, 0, 0, 0);
};

void QuadMesh::CreateIndexResource() {

#pragma region//IndexResourceを作成
    indexResource_ = DirectXCommon::CreateBufferResource(sizeof(uint32_t) * 6);
    //Viewを作成する IndexBufferView(IBV)

    //リソースの先頭アドレスから使う
    indexBufferView_.BufferLocation = indexResource_->GetGPUVirtualAddress();
    //使用するリソースのサイズはインデックス6つ分のサイズ
    indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
    //インデックスはuint32_tとする
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;
#pragma endregion

#pragma region//IndexResourceにデータを書き込む
    //インデックスリーソースにデータを書き込む
    indexResource_->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

    //右下
    //右上
    //左下
    //左上

    //頂点数を削減
    indexData_[0] = 0;
    indexData_[1] = 1;
    indexData_[2] = 2;

    indexData_[3] = 2;
    indexData_[4] = 3;
    indexData_[5] = 0;

    indexResource_->Unmap(0, nullptr);
#pragma endregion
}