#include "Arrow.h"
#include"Texture.h"
#include"TextureManager.h"
#include<numbers>
#include"Player.h"
#include"OBB.h"

Arrow::Arrow()
{
    quad_ = std::make_unique<QuadMesh>();
    quad_->Create(Texture::GetHandle(Texture::ARROW));
    //OBBのデバック描画の生成
    obbRenderer_ = std::make_unique<OBBRenderer>();
    kickObb_ = std::make_unique<OBB>();
}

void Arrow::Initialize() {
    // ワールド変換の初期化
    worldTransformParent_.Initialize();
    worldTransform_.parent_ = &worldTransformParent_;
    worldTransform_.Initialize();
    worldTransform_.scale_ = { 5.0f,5.0f,5.0f };
    worldTransform_.rotate_.x = std::numbers::pi_v<float> / 2.0f;
    worldTransform_.translate_.z = 2.5f;
    worldTransform_.translate_.y -= 1.5f;

    // OBBのデバッグ描画の初期化
    obbRenderer_->Initialize();
    kickObb_->halfSizes = { 2.5f,2.5f,2.5f };
}

void Arrow::Update(const Vector3& position, const float& scaleZ, const float& rotateY) {

    worldTransformParent_.translate_ = position;
    worldTransformParent_.rotate_.y = rotateY;
    worldTransformParent_.scale_.z = scaleZ * 0.002f;
    WorldTransformUpdate(worldTransformParent_);

    WorldTransformUpdate(worldTransform_);

    kickObb_->center = worldTransform_.GetWorldPosition();
    kickObb_->halfSizes.z = scaleZ * 0.005f;
    SetAxis(worldTransformParent_.rotate_, *kickObb_);

#ifdef _DEBUG
    EditOBB("kickOBB", worldTransformParent_.rotate_, *kickObb_);
    obbRenderer_->Update(GetKickAreaOBB());
#endif // _DEBUG
}

void Arrow::Draw(Camera& camera) {



    // 3Dモデル描画前処理
    quad_->PreDraw();
    // 3Dモデルを描画
    quad_->Draw(camera, worldTransform_.matWorld_, MaterialResource::NONE);

#ifdef _DEBUG
    obbRenderer_->Draw(camera);
#endif // _DEBUG


}

Arrow::~Arrow()
{
}

Vector3 Arrow::GetWorldPosition()const
{
    return  worldTransform_.GetWorldPosition();
}

OBB Arrow::GetKickAreaOBB()
{
    return *kickObb_;
}
