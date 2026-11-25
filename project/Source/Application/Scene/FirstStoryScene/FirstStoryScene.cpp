#include "FirstStoryScene.h"
#include"ModelManager.h"
#include "SceneManager.h"
#include"Input.h"
#include"Easing.h"
#include"TimeManager.h"
#include"Sound.h"
#include"../../GameObject/UI/Skip/Skip.h"
#include"InputBind.h"
#include"Model.h"
#include"DebugUI.h"
#include"PostProcessManager/PostProcessManager.h"
#include"LightingManager.h"

#include"FirstStoryZero/FirstStoryZero.h"
#include"FirstStoryOne/FirstStoryOne.h"


FirstStoryScene::FirstStoryScene()
{   // 現在のカメラを設定
    currentCamera_ = camera_.get();

    //スカイボックス
    skyboxObject3d_ = std::make_unique<SkyboxObject3d>();
    skyboxObject3d_->Create();
    skyboxObject3d_->SetTextureHandle(TextureFactory::SKYBOX_CLOUD_TEX);


    skip_ = std::make_unique<Skip>();
    storyZero_ = std::make_unique<FirstStoryZero>();
    storyOne_ = std::make_unique<FirstStoryOne>();
}

FirstStoryScene::~FirstStoryScene()
{

}

void FirstStoryScene::Initialize()

{

    Sound::StopAllSound();
    //カメラを初期化する
    camera_->Initialize();

    LevelEditor::GetInstance()->Load("FirstStoryScene_objectEditor", true);
    LevelEditor::GetInstance()->CreateObject(objects_);

    SetSceneChange();

    Sound::PlayBGM(SoundFactory::CITY, 0.125f);

    skip_->Initialize();

    //============//ストーリー//===============
    storyZero_->Initialize();
    storyOne_->Initialize();


    auto* gaussianFilter = PostProcessManager::GetInstance()->
        GetPostEffectMaterial(PostProcessManager::kModel)->
        GetMaterialGaussianFilter();

    gaussianFilter->sigma = 1.0f;
    gaussianFilter->kernel = 0;

    LightingManager::NoonDirLightInit();


}

void FirstStoryScene::Update()
{
    Debug();



#ifdef _RELEASE

    if (storyOne_->GetIsStoryOneEnd()) {
        //ゼロストーリー終了のフラグがが強制的にたち、ヴォイス終了を検知したら
        SceneChange();
    } else {

        if (InputBind::IsClick()) {

            if (skip_->GetIsSkip()) {
                storyOne_->Unload();
        
            } else {
                Sound::PlaySE(SoundFactory::FALL);
                skip_->SetIsDraw(true);
            }
        }
    }

#endif

    CameraUpdate();

    currentCamera_->UpdateMatrix();

    storyOne_->Update();

    if (storyOne_->GetIsStorZeroEnd()) {
        for (auto& obj : objects_) {
            obj->obj_->Update();
        }
    } else {
        storyZero_->Update();
    }
  
}

void FirstStoryScene::DrawModel()
{
    if (!storyOne_->GetIsStorZeroEnd()) {
        return;
    }

    skyboxObject3d_->Draw(*currentCamera_);

    for (auto& obj : objects_) {
        obj->obj_->Draw(kBlendModeNormal, kCullModeBack, kAll, false, TextureFactory::SKYBOX_CLOUD_TEX);
    }

     storyOne_->Draw();
}

void FirstStoryScene::DrawSprite()
{
    Sprite::PreDraw();

    if (!storyOne_->GetIsStorZeroEnd()) {
        storyZero_->Draw();
    }
  
    skip_->Draw();


}

void FirstStoryScene::SetSceneChange()
{
    sceneChange_->Initialize();
    sceneChange_->SetState(SceneChange::kFadeOut, 1.0f);
}

void FirstStoryScene::Debug()
{
#ifdef USE_IMGUI

    ImGui::Begin("Game");

    if (ImGui::Button("SwitchCamera")) {
        SwitchCamera();
    }

    ImGui::End();

    storyZero_->Debug();

#endif // !USE_IMGUI
}

void FirstStoryScene::CameraUpdate()
{

    const FirstStoryOne::PHASE  phase = storyOne_->GetPhase();
    EulerTransform transform;

    if (phase <= FirstStoryOne::PHASE::kFirst) {
        //定点カメラ始点
        transform = {
             .scale = {1.0f,1.0f,1.0f},
             .rotate = {0.0f,1.262f,-0.952f},
             .translate = {-1.5f,2.325f,-0.1012f}
        };
        camera_->SetFovAngleY(Camera::kFovAngle);

    } else if (phase == FirstStoryOne::PHASE::kSecond) {
        //玄関全体がうつる
        Vector3 translate = { 0.0f,0.75f,-0.0625f };
        //ポストに焦点がいく
        float time = storyOne_->GetLocalTimer();
        transform.scale = { 1.0f,1.0f,1.0f };
        transform.rotate = { 0.0f,0.0f,0.0f };
        transform.translate = Easing::EaseOutBack(translate, middlePos_, time);

    } else if (phase == FirstStoryOne::PHASE::kThird) {
        float time = storyOne_->GetLocalTimer();
        transform.translate = middlePos_;
        transform.scale = { 1.0f,1.0f,1.0f };
        transform.rotate = { 0.0f,0.0f,0.0f };
        camera_->SetFovAngleY(Easing::EaseInBack(Camera::kFovAngle, cameraFovAngle_, time));
    } else if (phase == FirstStoryOne::PHASE::kEnd) {
        transform.translate = middlePos_;
        transform.scale = { 1.0f,1.0f,1.0f };
        transform.rotate = { 0.0f,0.0f,0.0f };
    }

    camera_->SetTransform(transform);
}



void FirstStoryScene::SceneChange()
{
    sceneChange_->SetState(SceneChange::kFadeIn, 1.0f);
    SceneManager::SetNextScene("SecondStory");
}
