#include "TitleLogo.h"
#include "ModelManager.h"
#include "Model.h"
#include "MyEngine.h"
#include "Easing.h"
#include "Input.h"
#include "MatsumotoObj/MY_Utility.h"
#include <algorithm>

// Object3dはコピー禁止なので、vector<Object3d>ではなくvector<std::unique_ptr<Object3d>>を使う
TitleLogo::TitleLogo()
{
    body_.Create();
    body_.Initialize();

    logos_.resize(5);
    for (int i = 0; i < logos_.size(); ++i) {
        logos_[i] = std::make_unique<Object3d>();
        logos_[i]->Create();
        logos_[i]->Initialize();
        logos_[i]->worldTransform_.Parent(body_.worldTransform_);
    }

    logos_[0]->SetMesh(ModelManager::GetModel(ModelManager::TITLE_LOGO_1));
    logos_[1]->SetMesh(ModelManager::GetModel(ModelManager::TITLE_LOGO_2));
    logos_[2]->SetMesh(ModelManager::GetModel(ModelManager::TITLE_LOGO_3));
    logos_[3]->SetMesh(ModelManager::GetModel(ModelManager::TITLE_LOGO_4));
    logos_[4]->SetMesh(ModelManager::GetModel(ModelManager::TITLE_LOGO_5));
}

TitleLogo::~TitleLogo()
{
    logos_.clear();
}

void TitleLogo::Initialize()
{
    body_.worldTransform_.translate_ = { 0.0f,5.0f,0.0f };
    body_.worldTransform_.scale_ = { 2.0f,2.0f,2.0f };

    float radius = 1.3f; // 円の半径
    float angleStep = 2.0f * 3.14159265f / static_cast<float>(logos_.size());
    for (int i = 0; i < logos_.size(); i++) {
        float angle = i * angleStep;
        float x = radius * std::cos(angle);
        float y = radius * std::sin(angle);
        logos_[i]->worldTransform_.translate_ = { x, y, 0.0f };
        logos_[i]->worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
        logos_[i]->worldTransform_.rotate_ = { -3.14f * 0.5f, 0.0f, 0.0f }; // X軸回転
    }

	time_ = 0.0f;
	isActive = true;
	z = 0.0f;
}

void TitleLogo::Update()
{
	time_+= 0.016f;

    if (isActive) {
        body_.worldTransform_.translate_.z = MY_Utility::SimpleEaseIn(body_.worldTransform_.translate_.z, 0.0f, 0.1f);
    }
    else {
        body_.worldTransform_.translate_.z =MY_Utility::SimpleEaseIn(body_.worldTransform_.translate_.z, -25.0f, 0.1f);
    }

    float radius = 1.5f; // 円の半径
    float angleStep = 2.0f * 3.14159265f / static_cast<float>(logos_.size());
    for (int i = 0; i < logos_.size(); i++) {
        float angle = i * angleStep;
        float x = radius * std::cos(angle+ time_);
        float y = radius * std::sin(angle+ time_);
        logos_[i]->worldTransform_.translate_ = { x, y, 0.0f };
        logos_[i]->worldTransform_.scale_ = { 1.0f + std::sin(angle + time_) * 0.1f,1.0f + std::sin(angle + time_) * 0.1f,1.0f + std::sin(angle + time_) * 0.1f };
    }

	body_.Update();
    for (auto& logo : logos_) {
        logo->Update();
    }
}

void TitleLogo::Draw(Camera& camera)
{
    for (auto& logo : logos_) {
        logo->Draw(camera);
    }
}