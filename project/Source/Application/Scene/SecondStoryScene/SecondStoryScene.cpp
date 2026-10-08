#include "SecondStoryScene.h"
#include"ModelManager.h"
#include "SceneManager.h"
#include"Easing.h"
#include"TimeManager.h"
#include"Sound.h"
#include"../../GameObject/UI/Skip/Skip.h"
#include"InputBind.h"
#include"Model.h"
#include"DebugUI.h"
#include"PostProcessManager/PostProcessManager.h"
#include"LightingManager.h"

SecondStoryScene::SecondStoryScene()
{   // 現在のカメラを設定
    currentCamera_ = camera_.get();

    //スカイボックス
    skyboxObject3d_ = std::make_unique<SkyboxObject3d>();
    skyboxObject3d_->Create();
    skyboxObject3d_->SetTextureHandle(TextureFactory::SKYBOX_CLOUD_TEX);

    skip_ = std::make_unique<Skip>();
    //モデルを取得する
    Model* model = ModelManager::GetModel("player.gltf");
    aniObject_ = std::make_unique<AnimationObject3d>();
    aniObject_->Create();
    aniObject_->SetMeshAndMaterial(model);
    aniObject_->SetModelAndLoadAnimation(model);
    aniObject_->SetAnimation("InterCome");
}

SecondStoryScene::~SecondStoryScene()
{
    aniObject_->UnRegisterObject();
}

void SecondStoryScene::Initialize()
{
//カメラを初期化する
    camera_->Initialize();

    LevelEditor::GetInstance()->Load("FristStoryScene_objectEditor", true);
    LevelEditor::GetInstance()->CreateObject(objects_);

    SetSceneChange();

    Sound::PlayBGM(SoundFactory::CITY);

    skip_->Initialize();

    //体の位置初期化
    aniObject_->Initialize();

    //aniObject_->SetTranslate(pos);
    aniObject_->SetObjectName("SecondStoryPlayer");
    aniObject_->RegisterObject();
    aniObject_->SetTranslate({ -0.477f,0.0f,0.751f });
    aniObject_->SetAnimation("InterCome");
    aniObject_->UpdateAniTimer();

    aniObject_->Update();

    auto* gaussianFilter = PostProcessManager::GetInstance()->
        GetPostEffectMaterial(PostProcessManager::kModel)->
        GetMaterialGaussianFilter();

    gaussianFilter->sigma = 1.0f;
    gaussianFilter->kernel = 0;

    LightingManager::NoonLightInit();
}

void SecondStoryScene::Update()
{
    Debug();

#ifdef _RELEASE
    if (cameraTimer_ >= kEndTimer_) {
        SceneChange();
    } else {

        if (InputBind::IsClick()) {

            if (skip_->GetIsSkip()) {
                cameraTimer_ = kEndTimer_;
            } else {
                Sound::PlaySE(SoundFactory::FALL);
                skip_->SetIsDraw(true);
            }

        }
    }

#endif


    currentCamera_->UpdateMatrix();

    for (auto& obj : objects_) {
        obj->obj_->Update();
    }

    aniObject_->Update();
    //アニメーションタイマーのアップデート
    aniObject_->UpdateAniTimer();
}

void SecondStoryScene::DrawModel()
{

    skyboxObject3d_->Draw(*currentCamera_);

    for (auto& obj : objects_) {
        obj->obj_->Draw(kBlendModeNormal, kCullModeBack, kAll, false, TextureFactory::SKYBOX_CLOUD_TEX);
    }

     aniObject_->Draw();
    

}

void SecondStoryScene::DrawSprite()
{
    Sprite::PreDraw();

    skip_->Draw();
}

void SecondStoryScene::SetSceneChange()
{
    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 1.0f);
}

void SecondStoryScene::Debug()
{
#ifdef USE_IMGUI

    ImGui::Begin("Debug");

    if (ImGui::Button("SwitchCamera")) {
        SwitchCamera();
    }

    DebugUI::CheckObject3d(*aniObject_);
    ImGui::End();

#endif // !USE_IMGUI
}


void SecondStoryScene::SceneChange()
{
    sceneChange_->SetState(SceneChange::kFadeIn, 1.0f);
    SceneManager::SetNextScene("Game");
}
