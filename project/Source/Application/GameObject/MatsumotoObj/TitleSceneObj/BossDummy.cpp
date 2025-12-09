#include "BossDummy.h"
#include"ModelManager.h"
#include"Model.h"
#include"MyEngine.h"
#include"Easing.h"
#include<algorithm>
#include"Collision.h"
#include "Input.h"
#include"CollisionConfig.h"
#include"JsonFile.h"
#include "MatsumotoObj/MY_Utility.h"
#include"VibrateManager.h"

BossDummy::BossDummy() {
    body_.Create();

    //体のモデルを取得する
    models_[BODY] = ModelManager::GetModel(ModelManager::ENEMY_BODY);
    models_[WING_L] = ModelManager::GetModel(ModelManager::ENEMY_WING_L);
    models_[WING_R] = ModelManager::GetModel(ModelManager::ENEMY_WING_R);

    body_.SetMesh(models_[BODY]);

    // ===================//羽を追加しましたヨシダ//================================
    wingLPos_.Create();
    wingRPos_.Create();
    wingLPos_.SetMesh(models_[WING_L]);
    wingRPos_.SetMesh(models_[WING_R]);
    wingLPos_.worldTransform_.Parent(body_.worldTransform_);
    wingRPos_.worldTransform_.Parent(body_.worldTransform_);
    wingLPos_.worldTransform_.translate_ = { -1.0f,0.3f,0.0f };
    wingRPos_.worldTransform_.translate_ = { 1.0f,0.3f,0.0f };
    // ==========================================================================
}

BossDummy::~BossDummy() {
}

void BossDummy::Initialize() {
    body_.worldTransform_.translate_ = { 0.0f,1.5f,8.0f };
    body_.worldTransform_.rotate_ = { 0.0f,0.0f,0.0f };
    body_.worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
    body_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
    wingLPos_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
    wingRPos_.SetColor({ 1.0f,1.0f,1.0f,1.0f });
    timer_ = 0.0f;
    isAnimEnd_ = false;

    for (auto& [type, model] : models_) {
        model->ResetTextureHandle();
    }

}

void BossDummy::Update() {

    if (timer_ == 0.0f) {
        VibrateManager::SetTime(3.5f, 20000, 20000);
    }
    timer_ += 0.016f;
    body_.worldTransform_.rotate_.y = MY_Utility::SimpleEaseIn(body_.worldTransform_.rotate_.y, 3.14f, 0.1f);
    body_.SetColor(MY_Utility::SimpleEaseIn(body_.GetColor(), { 1.0f,0.0f,0.0f,1.0f }, 0.1f));
    wingLPos_.SetColor(MY_Utility::SimpleEaseIn(wingLPos_.GetColor(), { 1.0f,0.0f,0.0f,1.0f }, 0.1f));
    wingRPos_.SetColor(MY_Utility::SimpleEaseIn(wingRPos_.GetColor(), { 1.0f,0.0f,0.0f,1.0f }, 0.1f));
    body_.worldTransform_.scale_ = MY_Utility::SimpleEaseIn(body_.worldTransform_.scale_, { 1.3f, 1.3f, 1.3f }, 0.05f);
    body_.worldTransform_.translate_.x = sinf(timer_ * 30.0f) * 0.1f;
    body_.Update();

    wingLPos_.Update();
    wingRPos_.Update();

    Winging(2.0f);




    if (timer_ > 3.0f) {
        isAnimEnd_ = true;
    }

#ifdef USE_IMGUI
    DebugUI::CheckObject3d(body_, "bodyPos");
    DebugUI::CheckObject3d(wingLPos_, "wingLPos");
    DebugUI::CheckObject3d(wingRPos_, "wingRPos");
#endif
}

void BossDummy::Draw(Camera& camera, const LightMode& lightType) {
    body_.SetLightMode(lightType);
    wingLPos_.SetLightMode(lightType);
    wingRPos_.SetLightMode(lightType);

    body_.Draw(camera, kBlendModeNormal);
    wingLPos_.Draw(camera, kBlendModeNormal);
    wingRPos_.Draw(camera, kBlendModeNormal);
}

void BossDummy::Winging(const float& speed)
{
    float theta = timer_ * speed;
    wingLPos_.worldTransform_.rotate_.z = sinf(theta);
    wingRPos_.worldTransform_.rotate_.z = -sinf(theta);
}
