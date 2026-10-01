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

namespace {
    const float kEndTimer_ = 13.0f;
    const float kMiddleTime = 11.0f;
    const float kMiddle0Time = 6.0f;
}


FirstStoryScene::FirstStoryScene()
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

FirstStoryScene::~FirstStoryScene()
{
    aniObject_->UnRegisterObject();
}

void FirstStoryScene::Initialize()

{ 
    Sound::StopAllSound();
    
    //カメラを初期化する
    camera_->Initialize();
    //camera_->UpdateMatrix();
    cameraTimer_ = 0.0f;

    LevelEditor::GetInstance()->Load("FristStoryScene_objectEditor", true);
    LevelEditor::GetInstance()->CreateObject(objects_);

    SetSceneChange();

    for (auto& ringFlag : isInterComeRing_) {
        ringFlag = false;
    }

    isHorror_ = false;

    Sound::PlayBGM(SoundFactory::CITY);

    skip_->Initialize();


    //体の位置初期化
    aniObject_->Initialize();

    //aniObject_->SetTranslate(pos);
    aniObject_->SetObjectName("FirstStoryPlayer");
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

void FirstStoryScene::Update()
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



    CameraUpdate();


    //if (isDebugCameraActive_) {

    //}/* else {
    //    //プレイヤーの目の位置をカメラの位置とする
    //    camera_->SetWorldMatrix(player_->GetEyeMatrix());
    //    camera_->SetFovAngleY(Easing::EaseOutBack(Camera::kFovAngle, Camera::kFovAngle * 0.5f, player_->GetZoomTimer()));
    //    camera_->UpdateViewProjectionMatrix();
    //}
    //*/

    currentCamera_->UpdateMatrix();

    for (auto& obj : objects_) {
        obj->obj_->Update();
    }

    aniObject_->Update();
    //アニメーションタイマーのアップデート
    aniObject_->UpdateAniTimer();
}

void FirstStoryScene::DrawModel()
{

    skyboxObject3d_->Draw(*currentCamera_);

    for (auto& obj : objects_) {
        obj->obj_->Draw(kBlendModeNormal, kCullModeBack, kAll, false, TextureFactory::SKYBOX_CLOUD_TEX);
    }

    if (cameraTimer_ <= kMiddleTime) {
        //中間時間いないの時はびゅがする
        aniObject_->Draw();
    }

}

void FirstStoryScene::DrawSprite()
{
    Sprite::PreDraw();

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

    ImGui::Begin("Debug");

    if (ImGui::Button("SwitchCamera")) {
        SwitchCamera();
    }

    DebugUI::CheckObject3d(*aniObject_);
    ImGui::End();

#endif // !USE_IMGUI
}

void FirstStoryScene::CameraUpdate()
{
    cameraTimer_ += TimeManager::DeltaTime();
    EulerTransform transform;

    if (cameraTimer_ <= kMiddle0Time) {

        //定点カメラ始点
       transform = {
            .scale = {1.0f,1.0f,1.0f},
            .rotate = {0.0f,1.262f,-0.952f},
            .translate = {-1.5f,2.325f,-0.1012f}
        };
       camera_->SetFovAngleY( Camera::kFovAngle);
    
       if (cameraTimer_ >= 4.5f) {
           PlayInterCome(2);
       }else if (cameraTimer_ >= 3.5f) {
           PlayInterCome(1);
       }else if (cameraTimer_ >= 1.0f) {
           PlayInterCome(0);
       }

    } else if (cameraTimer_ <= kMiddleTime) {
        if (!isHorror_) {
            //ホラー音を流す
            Sound::PlaySE(SoundFactory::HORROR2);
            isHorror_ = true;
        }
        //玄関全体がうつる
        Vector3 translate = { 0.0f,0.75f,-0.0625f };
        //ポストに焦点がいく
        float time = TimeManager::GetLocalTimer(cameraTimer_, kMiddle0Time, kMiddleTime);
        transform.scale = { 1.0f,1.0f,1.0f };
        transform.rotate = { 0.0f,0.0f,0.0f };
        transform.translate = Easing::EaseOutBack(translate, middlePos_, time);


    } else if(cameraTimer_<= kEndTimer_) {

        float time = TimeManager::GetLocalTimer(cameraTimer_, kMiddleTime, kEndTimer_);
        transform.translate = middlePos_;
        transform.scale = { 1.0f,1.0f,1.0f };
        transform.rotate = { 0.0f,0.0f,0.0f };
        camera_->SetFovAngleY(Easing::EaseInBack(Camera::kFovAngle, cameraFovAngle_, time));
    } else {
        transform.translate = middlePos_;
        transform.scale = { 1.0f,1.0f,1.0f };
        transform.rotate = { 0.0f,0.0f,0.0f };


    }
    camera_->SetTransform(transform);
}

void FirstStoryScene::PlayInterCome(const int32_t index)
{
    if (static_cast<size_t>(index) < isInterComeRing_.size()) {
        if (!isInterComeRing_[index]) {
            //いちどどめてから
        
            Sound::PlaySE(SoundFactory::INTERCOM);
            isInterComeRing_[index] = true;
        }
    }


}

void FirstStoryScene::SceneChange()
{
    sceneChange_->SetState(SceneChange::kFadeIn, 1.0f);
    SceneManager::SetNextScene("Game");
}
