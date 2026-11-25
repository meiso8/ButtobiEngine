#include "FirstStoryZero.h"
#include"Sprite.h"
#include"TimeManager.h"
#include"DebugUI.h"
#include"Easing.h"
#include"Random.h"

FirstStoryZero::FirstStoryZero()
{
    const float height = static_cast<float>(Window::GetClientHeight());
    const float width = static_cast<float>(Window::GetClientWidth());
    const float halfWidth = width * 0.5f;
    const float halfHeight = height * 0.5f;
    const float heightPos = halfHeight + 16.0f;

    aniSprites_[kCalendar].pos = { 1132.0f, heightPos };

    aniSprites_[kGirl].pos = { 680.0f, heightPos };
    aniSprites_[kHand].pos = { 256.0f, heightPos };

    handShakeStartPos_ = aniSprites_[kHand].pos;
    handShakeEndPos_ = aniSprites_[kHand].pos;


    Texture::LoadAndMapHandle("Resource/Textures/white1x1.png", TextureFactory::WHITE_1X1);
    std::string directoryName = "Resource/Textures/LostContact/";
    int textures[3] = { 0 };

    textures[kCalendar] = Texture::LoadAndGetIndex(directoryName + "calendar.png");
    textures[kGirl] = Texture::LoadAndGetIndex(directoryName + "girl.png");
    textures[kHand] = Texture::LoadAndGetIndex(directoryName + "hand.png");

    for (int i = 0; i < aniSprites_.size(); ++i) {

        aniSprites_[i].sprite = std::make_unique<Sprite>();
        aniSprites_[i].sprite->Create(TextureFactory::WHITE_1X1, Vector2{ halfWidth, heightPos });

        aniSprites_[i].sprite->SetSRVTextureHandle(textures[i]);
        aniSprites_[i].sprite->SetPosition(aniSprites_[i].pos);


        aniSprites_[i].sprite->SetAnchorPoint(Vector2{ 0.5f,0.5f });
        aniSprites_[i].sprite->AdjustTextureSize();
        aniSprites_[i].sprite->Update();
    }


}
FirstStoryZero::~FirstStoryZero()
{

}
void FirstStoryZero::Initialize()
{
    for (int i = 0; i < aniSprites_.size(); ++i) {
        aniSprites_[i].timer = 0.0f;
        aniSprites_[i].sprite->SetPosition(aniSprites_[i].pos);
        aniSprites_[i].sprite->SetRotate(0.0f);
        aniSprites_[i].sprite->SetScale({ 1.0f,1.0f });

    }

    handShakeStartPos_ = aniSprites_[kHand].pos;
    handShakeEndPos_ = aniSprites_[kHand].pos;

    aniSprites_[kGirl].sprite->SetScale({ 0.5f ,0.5f });
    aniSprites_[kCalendar].sprite->SetColor({ 1.0f,1.0f,1.0f,0.0f });
}

void FirstStoryZero::Update()
{
    aniSprites_[kHand].timer += TimeManager::DeltaTime();

    if (aniSprites_[kHand].timer <= 2.0f) {
        const float localHandTimer = TimeManager::GetLocalTimer(aniSprites_[kHand].timer, 0.0f, 2.0f);
        aniSprites_[kHand].sprite->SetPosition(
            Easing::EaseOutBack<Vector2>(
                Vector2{ -aniSprites_[kHand].sprite->GetSize().x * 0.5f,   aniSprites_[kHand].pos.y },
                aniSprites_[kHand].pos,
                localHandTimer
            )
        );
    } else {
        if (aniSprites_[kHand].timer >= 2.5f) {
            Random random(-10.0f, 10.0f);
            handShakeStartPos_ = aniSprites_[kHand].sprite->GetPosition();
            handShakeEndPos_ =
                aniSprites_[kHand].pos
                + Vector2{ random.Get(), random.Get() };

            aniSprites_[kHand].timer = 2.0f;
        }

        const float localHandTimer = TimeManager::GetLocalTimer(aniSprites_[kHand].timer, 2.0f, 2.5f);
        aniSprites_[kHand].sprite->SetPosition(Lerp(handShakeStartPos_, handShakeEndPos_, localHandTimer));

    }


    if (aniSprites_[kGirl].timer < 2.0f) {

        aniSprites_[kGirl].timer += TimeManager::DeltaTime();
        aniSprites_[kGirl].timer = std::clamp(aniSprites_[kGirl].timer, 0.0f, 2.0f);

        if (aniSprites_[kGirl].timer >= 1.0f) {
            const float localGirlTimer = TimeManager::GetLocalTimer(aniSprites_[kGirl].timer, 1.0f, 2.0f);
            const float scale = Easing::EaseInOutBack(0.5f, 1.0f, localGirlTimer);
            aniSprites_[kGirl].sprite->SetScale({ scale ,scale });
        }
    }


    if (aniSprites_[kCalendar].timer < 5.0f) {
        aniSprites_[kCalendar].timer += TimeManager::DeltaTime();
        aniSprites_[kCalendar].timer = std::clamp(aniSprites_[kCalendar].timer, 0.0f, 5.0f);

        const float localCalenderTimer = TimeManager::GetLocalTimer(aniSprites_[kCalendar].timer, 0.0f, 5.0f);

        const float alpha = Easing::EaseInElastic(0.0f, 1.0f, localCalenderTimer);
        aniSprites_[kCalendar].sprite->SetColor({ 1.0f,1.0f,1.0f,alpha });
    }

    for (auto& sprite : aniSprites_) {
        sprite.sprite->Update();
    }
}

void FirstStoryZero::Draw()
{


    for (int i = 0; i < aniSprites_.size(); ++i) {
        if (i == kGirl && aniSprites_[kGirl].timer < 1.0f) {
            continue;
        }
        aniSprites_[i].sprite->Draw();
    }

}

void FirstStoryZero::Debug() {

#ifdef USE_IMGUI
    ImGui::Begin("FirstStorySprite");
    int i = 0;
    for (auto& sirpte : aniSprites_) {
        //安全処理
        ImGui::PushID(i++);
        DebugUI::CheckSprite(*sirpte.sprite, std::to_string(i).c_str());
        ImGui::PopID();
    }
    ImGui::End();
#endif

}
