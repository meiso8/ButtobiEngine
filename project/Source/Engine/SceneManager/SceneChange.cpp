#include "SceneChange.h"
#include"Window.h"
#include"Easing.h"
#include"ImGuiClass.h"
#include"TimeManager.h"
#include"PostProcessManager/PostProcessManager.h"

std::unordered_map<SceneChange::State, PFunc>SceneChange::StatesUpdate_ =
{
    {State::kFadeIn, &SceneChange::FadeIn},
    {State::kFadeOut, &SceneChange::FadeOut},

};

SceneChange::~SceneChange()
{
}

SceneChange::SceneChange()
{
    sprite_ = std::make_unique<Sprite>();
    sprite_->Create(TextureFactory::WHITE_1X1, { 0.0f,0.0f }, { 0.0f,0.0f,0.0f,1.0f });
    sprite_->SetSize(Vector2{ static_cast<float>(Window::GetClientWidth()), static_cast<float>(Window::GetClientHeight()) });
}

void SceneChange::SetState(State state, const float endTime, const bool skipEffect)
{
    if (state == kUnKnown) { return; }
    if (state == state_) { return; }
    state_ = state;
    timer_ = 0.0f;
    endTime_ = endTime;
    isSkipEffect_ = skipEffect;
}

void SceneChange::Initialize()
{
    timer_ = 0.0f;
    state_ = kUnKnown;
    isSkipEffect_ = false;
}

void SceneChange::FadeOut()
{

    if (isSkipEffect_) {
        return;
    }

    float time = 1.0f - timer_ / endTime_;
    sprite_->SetColor({ 0.0f,0.0f,0.0f,time });

    auto* grayScaleMaterial = PostProcessManager::GetInstance()->
        GetPostEffectMaterial(PostProcessManager::kModel)->
        GetMaterialGrayScale();
    grayScaleMaterial->useEffect = true;
    grayScaleMaterial->type = PostEffectMaterial::RenderTextureEffectType::kGrayScale;
    grayScaleMaterial->effectVol = EaseOutQuadT(time);
    auto* mosaic = PostProcessManager::GetInstance()->GetPostEffectMaterial(PostProcessManager::kModel)->GetMaterialForMosaic();
    mosaic->useMosaic = true;
    mosaic->size = { Lerp(Window::GetClientWidth(),32,time) };
}

void SceneChange::FadeIn()
{
    
    if (isSkipEffect_) {
        return;
    }

    float time = timer_ / endTime_;
    sprite_->SetColor({ 0.0f,0.0f,0.0f,time });

    auto* grayScaleMaterial = PostProcessManager::GetInstance()->
        GetPostEffectMaterial(PostProcessManager::kModel)->
        GetMaterialGrayScale();
    grayScaleMaterial->useEffect = true;
    
    grayScaleMaterial->type = PostEffectMaterial::RenderTextureEffectType::kGrayScale;
    grayScaleMaterial->effectVol = EaseInQuadT(time);


    auto* mosaic = PostProcessManager::GetInstance()->GetPostEffectMaterial(PostProcessManager::kModel)->GetMaterialForMosaic();
    mosaic->useMosaic = true;
    mosaic->size = { Lerp(Window::GetClientWidth(),32,time) };
}



void SceneChange::SwitchScene()
{
    if (state_ == kSceneStart || state_ == kSceneEnd) {
        state_ = kUnKnown;

    }
    if (state_ == kFadeOut ) {
        state_ = kSceneStart;
        sprite_->SetColor({ 0.0f,0.0f,0.0f,0.0f});
        auto* grayScaleMaterial = PostProcessManager::GetInstance()->
            GetPostEffectMaterial(PostProcessManager::kModel)->
            GetMaterialGrayScale();
        grayScaleMaterial->useEffect = false;
       
        PostProcessManager::GetInstance()->GetPostEffectMaterial(PostProcessManager::kModel)->GetMaterialForMosaic()->useMosaic = false;
    }
    if (state_ == kFadeIn ) {
        state_ = kSceneEnd;
    }

    //シーン変更時にフェードインアニメーション設定の初期化をする
    isSkipEffect_ = false;

}

void SceneChange::Update()
{
    TimerUpdate();

    if (state_ == kUnKnown || state_ == kSceneStart || state_ == kSceneEnd) { return; }

    (this->*StatesUpdate_[state_])();
}

void SceneChange::TimerUpdate()
{
    if (timer_ >= endTime_) { return; }

    timer_+= TimeManager::DeltaTime();

    if (timer_ >= endTime_) {
        timer_ = endTime_;
        SwitchScene();
    }

}

void SceneChange::Draw()
{
    if (state_ == kUnKnown) { return; }

    if (sprite_ == nullptr) {
        return;
    }
    sprite_->PreDraw();
    sprite_->Draw();
}
