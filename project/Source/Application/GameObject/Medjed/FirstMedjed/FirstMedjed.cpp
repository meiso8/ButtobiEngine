#include "FirstMedjed.h"
#include"DebugUI.h"
#include"ModelManager.h"
#include"Model.h"
#include"TimeManager.h"
#include"Easing.h"
#include "TransformAni/TransformAni.h"
namespace {
    constexpr float kAniTime = 5.0f;
    constexpr float kApperTime = 2.0f;
}

FirstMedjed::FirstMedjed()
{
    Model* model = ModelManager::GetModel("medjed.gltf");
    aniObj_ = std::make_unique<AnimationObject3d>();
    aniObj_->Create();
    aniObj_->SetMeshAndMaterial(model);
    aniObj_->SetTemperature(1.0f);
    aniObj_->SetModelAndLoadAnimation(model);
}

FirstMedjed::~FirstMedjed()
{

}

void FirstMedjed::Initialize()
{
    aniTimer_ = 0.0f;

    isHide_ = false;
    isAppear_ = false;

    aniObj_->Initialize();
    aniObj_->SetMaskVol(0.0f);
    //アニメーションをセットするよ
    aniObj_->SetAnimation("Round");
    aniObj_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
    aniObj_->SetObjectName("FirstMedjed");
    aniObj_->SetRotate({0.0f,Math::kPi,0.0f});
    aniObj_->RegisterObject();
}

void FirstMedjed::Update()
{

    if (isHide_) {
        return;
    }

    if (isAppear_) {
        //アニメーションの更新
        aniTimer_ += TimeManager::DeltaTime();
        aniTimer_ = std::clamp(aniTimer_, 0.0f, kAniTime);

        if (aniTimer_ <= kApperTime) {
            aniObj_->SetTranslate(startPos_);
            aniObj_->SetMaskVol(Easing::EaseInOut(1.0f, 0.0f, TimeManager::GetLocalTimer(aniTimer_, 0.0f, kApperTime)));
        } else {
            aniObj_->SetAnimation("Walk");
            aniObj_->SetMaskVol(0.0f);
            aniObj_->SetTranslate(Lerp(startPos_,endPos_, TimeManager::GetLocalTimer(aniTimer_, kApperTime, kAniTime)));
            TransformAni::LookTarget(aniObj_->GetWorldTransform(), endPos_);
        }

        if (aniTimer_ >= kAniTime) {
            isHide_ = true;
        }

        aniObj_->UpdateAniTimer();
        aniObj_->Update();
    }


#ifdef USE_IMGUI

    ImGui::Begin("FirstMedjed");
    ImGui::Checkbox("isHide", &isHide_);
    ImGui::Checkbox("isAppear", &isAppear_);

    DebugUI::CheckObject3d(*aniObj_,"FirstMedjedObj");

    ImGui::End();

#endif
}

void FirstMedjed::Draw()
{

    if (isHide_) {
        return;
    }

    if (!isAppear_) {
        return;
    }

    aniObj_->Draw();
}

void FirstMedjed::SetStartPosAndEndPos(const Vector3& startPos, const Vector3& endPos)
{
    startPos_ = startPos;
    endPos_ = endPos;  
}
