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
    body_.worldTransform_.translate_ = { 0.0f,3.0f,15.0f };
    body_.worldTransform_.scale_ = { 1.0f,1.0f,1.0f };

    float logoSpacing = 1.5f;
    for (int i = 0; i < logos_.size(); i++) {
        logos_[i]->worldTransform_.translate_ = { 0.0f,static_cast<float>(i) * logoSpacing,0.0f };
        logos_[i]->worldTransform_.scale_ = { 1.0f,1.0f,1.0f };
    }
}

void TitleLogo::Update()
{
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