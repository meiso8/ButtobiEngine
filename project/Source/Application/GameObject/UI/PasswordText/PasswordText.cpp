#include "PasswordText.h"
#include"FreeTypeManager/FreeTypeManager.h"
#include"Sprite.h"
#include"Input.h"
#include"SoundManager/SoundManager.h"
#include"InputBind.h"
#include"Sound.h"



bool PasswordText::isActive_ = true;
PasswordText::PasswordText()
{
    uint32_t meiryob = FreeTypeManager::CreateFace("Resource/Fonts/meiryob.ttc", 0);
    uint32_t handle = FreeTypeManager::CreateFace("Resource/Fonts/NotoSansEgyptianHieroglyphs-Regular.ttf", 0);

    infoText_.Initialize(meiryob);

    //情報テキストa
    infoText_.SetString(U"サイトからチケットを購入してください");
    infoText_.SetPosition({ 640, 360 });
    infoText_.SetColor({ 1, 1, 1, 1 });
    infoText_.SetAlign(Text::TextAlign::Center);
    infoText_.SetBlendMode(BlendMode::kBlendModeNormal);

    float width = static_cast<float>(Window::GetClientWidth());
    float height = static_cast<float>(Window::GetClientHeight());

    webSprite_ = std::make_unique<Sprite>();
    webSprite_->Create(TextureFactory::WEB_SITE, { width * 0.5f,256.0f });
    webSprite_->SetAnchorPoint({ 0.5f,0.5f });

    //ネットワーク
    netWork_ = std::make_unique<Network>();
}

void PasswordText::Initialize()
{
    netWork_->Initialize();
    isUnLock_ = false;
    isActive_ = false;
}

void PasswordText::Update()
{

    if (!isActive_) {
        //非アクティブの時はreturn
        return;
    }
    if (isUnLock_) {

        isActive_ = false;
        //ロックが解除されたら今後処理を行わない
        return;
    }

    if (IsCollision(*webSprite_, *curPosPtr_)) {
        webSprite_->SetColor({ COLOR::RED });

        if (InputBind::IsClick()) {
            //// 既定のブラウザでURLを開く
            //ShellExecuteW(
            //    nullptr,            // 親ウィンドウのハンドル（nullptrでも可）
            //    L"open",            // 実行する操作
            //    L"https://meiso-labyrinth.netlify.app/", // 開きたいURL
            //    nullptr,            // パラメータ（URLの場合は不要）
            //    nullptr,            // 作業ディレクトリ（URLの場合は不要）
            //    SW_SHOWNORMAL       // 開くときのウィンドウの状態
            //);

            netWork_->OnButtonClicked();

        }
    } else {
        webSprite_->SetColor({ COLOR::WHITE });
    }


    if (!isUnLock_) {
        if (netWork_->GetIsTicketGet()) {
            //成功音
            SoundManager::PlayCorrectSE();
            isUnLock_ = true;
        }
    }

}

void PasswordText::Draw()
{
    if (!isActive_) {
        return;
    }

    Sprite::PreDraw();
    webSprite_->Draw();
    infoText_.Draw();


}
