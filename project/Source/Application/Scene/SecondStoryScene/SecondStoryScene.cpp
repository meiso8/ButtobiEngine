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
#include"TreeGenerator/TreeGenerator.h"

SecondStoryScene::SecondStoryScene()
{  
    // 現在のカメラを設定
    currentCamera_ = camera_.get();

    //スカイボックス
    skyboxObject3d_ = std::make_unique<SkyboxObject3d>();
    skyboxObject3d_->Create();
    skyboxObject3d_->SetTextureHandle(TextureFactory::SKYBOX_CLOUD_TEX);

    skip_ = std::make_unique<Skip>();
    //モデルを取得する
    Model* model = ModelManager::LoadModelAndGet("Resource/Models/Gentleman/gentleman.gltf");
    aniObject_ = std::make_unique<AnimationObject3d>();
    aniObject_->Create();
    aniObject_->SetMeshAndMaterial(model);
    aniObject_->SetModelAndLoadAnimation(model);
    aniObject_->SetAnimation("Bye-Bye");

    //木のランダム生成クラス
    treeGenerator_ = std::make_unique<TreeGenerator>();

}

SecondStoryScene::~SecondStoryScene()
{
    aniObject_->UnRegisterObject();
}

void SecondStoryScene::Initialize()
{
//カメラを初期化する
    camera_->Initialize();
    camera_->SetTranslate({ 0.0f,1.586f,-1.268f });
    camera_->SetFovAngleY(0.4f);

    LevelEditor::GetInstance()->Load("SecondStoryScene_objectEditor", true);
    LevelEditor::GetInstance()->CreateObject(objects_);

    SetSceneChange();

    Sound::PlayBGM(SoundFactory::CITY);

    skip_->Initialize();

    //体の位置初期化
    aniObject_->Initialize();

    //aniObject_->SetTranslate(pos);
    aniObject_->SetObjectName("SecondStoryPlayer");
    aniObject_->RegisterObject();
    aniObject_->SetTranslate({ 0.0f,0.0f,0.0f });
    aniObject_->SetAnimation("Bye-Bye");
    //Y軸回転させる
    aniObject_->SetRotate({ 0.0f,Math::kPi,0.0f });


    walkAnimationTimer_ = 0.0f;

    auto* gaussianFilter = PostProcessManager::GetInstance()->
        GetPostEffectMaterial(PostProcessManager::kModel)->
        GetMaterialGaussianFilter();

    gaussianFilter->sigma = 1.0f;
    gaussianFilter->kernel = 0;

    auto* grayScale=  PostProcessManager::GetInstance()->GetPostEffectMaterial(PostProcessManager::kModel)->GetMaterialGrayScale();
    grayScale->useEffect = true;
    grayScale->type = PostEffectMaterial::RenderTextureEffectType::kSepia;
    grayScale->effectVol = 0.0f;
    LightingManager::NoonLightInit();

    //木の生成
    treeGenerator_->Initialize();

}

void SecondStoryScene::Update()
{
    Debug();

#ifdef _RELEASE
    if (walkAnimationTimer_ >= kEndTimer_) {
        //終わりを迎えたらシーンチェンジ
        SceneChange();
    } else {

        if (InputBind::IsClick()) {

            if (skip_->GetIsSkip()) {
                walkAnimationTimer_ = kEndTimer_;
            } else {
                Sound::PlaySE(SoundFactory::FALL);
                skip_->SetIsDraw(true);
            }

        }
    }

#endif

    if (aniObject_->GetCurrentAnimation() == "Bye-Bye") {

    }    else if (aniObject_->GetCurrentAnimation() == "Walk") {
        if (walkAnimationTimer_>=2.0f&& walkAnimationTimer_ <= 5.0f) {
            camera_->SetFovAngleY(Lerp(0.4f, 1.8f, TimeManager::GetLocalTimer(walkAnimationTimer_, 2.0f, 5.0f)));
        }
        
    }

    currentCamera_->UpdateMatrix();

    for (auto& obj : objects_) {
        obj->obj_->Update();
    }


    //アニメーションタイマーのアップデート
    aniObject_->UpdateAniTimer(aniObject_->GetCurrentAnimation() == "Walk" ? true : false);

    aniObject_->Update();



    //グレースケール
    auto* grayScale = PostProcessManager::GetInstance()->GetPostEffectMaterial(PostProcessManager::kModel)->GetMaterialGrayScale();

    if (aniObject_->GetCurrentAnimation() == "Bye-Bye") {

        //バイバイが終わったら
        if (aniObject_->IsAnimEnd()) {
            //歩く
            aniObject_->SetAnimation("Walk");
            grayScale->useEffect = true;
            grayScale->type = PostEffectMaterial::RenderTextureEffectType::kSepia;

        }

    } else if (aniObject_->GetCurrentAnimation() == "Walk") {

        if (walkAnimationTimer_ <= kEndTimer_) {
            walkAnimationTimer_ += TimeManager::DeltaTime();


            if (walkAnimationTimer_ <= 1.0f) {
                float localTimer = TimeManager::GetLocalTimer(walkAnimationTimer_, 0.0f, 1.0f);
                //アイドルだったら
                aniObject_->SetRotate({ 0.0f,Lerp(Math::kPi,0.0f,localTimer),0.0f });
                grayScale->effectVol = Lerp(0.0f, 1.0f, localTimer);
            } else {
                aniObject_->SetRotate({ 0.0f,0.0f,0.0f });
                aniObject_->SetTranslate({ 0.0f,0.0f,Lerp(0.0f,20.0f,TimeManager::GetLocalTimer(walkAnimationTimer_,1.0f,kEndTimer_)) });
                grayScale->effectVol = 1.0f;
            }

        }
  

    }

    treeGenerator_->Update();
}

void SecondStoryScene::DrawModel()
{

    skyboxObject3d_->Draw(*currentCamera_);

    for (auto& obj : objects_) {
        obj->obj_->Draw(kBlendModeNormal, kCullModeBack, kAll, false, TextureFactory::SKYBOX_CLOUD_TEX);
    }

     treeGenerator_->Draw();

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
    aniObject_->ImGuiDebugBone();
    ImGui::End();

    treeGenerator_->Debug();

#endif // !USE_IMGUI
}


void SecondStoryScene::SceneChange()
{
    sceneChange_->SetState(SceneChange::kFadeIn, 1.0f,true);
    SceneManager::SetNextScene("Game");
}
