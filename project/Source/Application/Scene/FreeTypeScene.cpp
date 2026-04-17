#include "FreeTypeScene.h"
#include"Input.h"
#include"AABB.h"
#include"DrawGrid.h"
FreeTypeScene::FreeTypeScene()
{

    handle_ = FreeTypeManager::CreateFace("Resource/Fonts/NotoSansEgyptianHieroglyphs-Regular.ttf", 0);
    FreeTypeManager::SetPixelSizes(handle_, 32, 32);
    text_.Initialize(handle_);
    pressSpaceText_.Initialize(handle_);

    text_.SetString(U"\U00013000ButtobiEngine");
    text_.SetPosition({ 640, 360 });
    text_.SetColor({ 1, 1, 1, 1 });
    text_.SetAlign(TextAlign::Center);
    text_.SetBlendMode(BlendMode::kBlendModeAdd);

    pressSpaceText_.SetString(U"SPACE");
    pressSpaceText_.SetPosition({ 640, 360 + 128 });
    pressSpaceText_.SetColor({ 1, 1, 1, 1 });
    pressSpaceText_.SetAlign(TextAlign::Center);
    pressSpaceText_.SetBlendMode(BlendMode::kBlendModeAdd);
    sprite_ = std::make_unique<Sprite>();
    sprite_->Create(TextureFactory::ART1, { 0,0 });
    sprite_->SetSize({ 1280,720 });


    skyBoxObj_ = std::make_unique<SkyboxObject3d>();
    skyBoxObj_->Create();
    

}

void FreeTypeScene::Initialize()
{
    camera_->Initialize();
    currentCamera_ = camera_.get();

}

void FreeTypeScene::Update()
{
    //入力受付
    //for (char32_t ch : Input::GetInputChars()) {
    //    if (ch == U'\b') {
    //        if (!inputText_.empty()) inputText_.pop_back();
    //    } else if (ch >= 0x20) {
    //        inputText_ += ch;
    //    }
    //    text_.SetString(inputText_);
    //}

    //デバック
    text_.Debug();

    if (inputText_ == U"next") {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
        SceneManager::SetNestScene("Title");
    }

    if (Input::IsTriggerKey(DIK_SPACE)) {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
        SceneManager::SetNestScene("Title");
    }

#ifdef _DEVELOP
    if (Input::IsTriggerKey(DIK_F1)) {
        SwitchCamera();
    }
#endif //_DEVELOP


    
    currentCamera_->UpdateMatrix();

}

void FreeTypeScene::Draw()
{

#ifdef _DEVELOP
    // デバッグカメラ
    DrawGrid::Draw(*currentCamera_);
#endif //_DEVELOP
    
    skyBoxObj_->Draw(*currentCamera_);

    //Sprite::PreDraw();
    //sprite_->Draw();
    text_.Draw();
    pressSpaceText_.Draw();

    sceneChange_->Draw();

}
