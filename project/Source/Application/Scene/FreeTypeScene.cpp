#include "FreeTypeScene.h"
#include"Input.h"
FreeTypeScene::FreeTypeScene()
{


    handle_ = FreeTypeManager::CreateFace("Resource/Fonts/NotoSansEgyptianHieroglyphs-Regular.ttf", 0);
    FreeTypeManager::SetPixelSizes(handle_, 64, 64);
    text_.Initialize(handle_);
    text_.SetString(U"Medjed");
    text_.SetPosition({ 400, 300 });
    text_.SetColor({ 1, 1, 1, 1 });
    text_.SetAlign(TextAlign::Center);
    text_.SetBlendMode(BlendMode::kBlendModeAdd);
}

void FreeTypeScene::Initialize()
{
}

void FreeTypeScene::Update()
{
    //入力受付
    for (char32_t ch : Input::GetInputChars()) {
        if (ch == U'\b') {
            if (!inputText_.empty()) inputText_.pop_back();
        } else if (ch >= 0x20) {
            inputText_ += ch;
        }
        text_.SetString(inputText_);
    }

    //デバック
    text_.Debug();

    if (inputText_== U"next") {
        sceneChange_->SetState(SceneChange::kFadeIn, 30);
        SceneManager::SetNestScene("Title");
    }
}

void FreeTypeScene::Draw()
{
    text_.Draw();
    sceneChange_->Draw();
}
