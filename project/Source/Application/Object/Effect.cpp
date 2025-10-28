#include "Effect.h"
#include"Model.h"
#include"Input.h"
#include"Camera/Camera.h"


Effect::Effect(const WorldTransform& parent)
{
    // 引数として受け取ったデータをメンバ変数に記録する
    model_ = new Model();
    model_->Create(ModelManager::EFFECT);
    // ワールド変換の初期化
    worldTransform_.Initialize();
    worldTransform_.parent_ = &parent;
    worldTransform_.scale_ = { 2.0f,2.0f,2.0f };
    worldTransform_.translate_.y -= 1.5f;
}

void Effect::Initialize() {

}

void Effect::Update(const bool& isCharge, const Vector4& color) {

    if (isCharge) {
        model_->GetUVTransform().translate.y -= InverseFPS * 2.0f;
        model_->SetColor(color);
    } else {
        model_->GetUVTransform().translate.y += InverseFPS * 6.0f;
        model_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
    }

    model_->UpdateUV();

    WorldTransformUpdate(worldTransform_);

}

void Effect::Draw(Camera& camera) {

    // 3Dモデル描画前処理
    model_->PreDraw(kBlendModeAdd);
    // 3Dモデルを描画
    model_->Draw(camera, worldTransform_.matWorld_, MaterialResource::NONE);

}

Effect::~Effect()
{
    delete model_;
}
