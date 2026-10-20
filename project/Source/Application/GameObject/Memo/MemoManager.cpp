#include "MemoManager.h"
#include"JsonFile/JsonFile.h"
#include"CollisionManager.h"
#include"InputBind.h"
#include"Sound.h"
#include<algorithm>
#include"TimeManager.h"
#include"FreeTypeManager/FreeTypeManager.h"
#include"DebugUI.h"
#include<utf8.h>

bool MemoManager::isLookItem_ = false;

namespace {
    float maxSpriteScale_ = { 2.0f };
    float minSpriteScale_ = { 1.0f };
}

std::string MemoManager::GetFilePath(const MemoType type)
{

    std::string fileName = "textMemo";

    switch (type) {
    case MEMO1: fileName = "memo01";   break;
    case MEMO2:fileName = "memo02"; break;
    case MEMO3:fileName = "memo03"; break;
    case MEMO4:fileName = "memo04"; break;
    case MEMO5:fileName = "memo05"; break;
    case BOOK1:fileName = "book";  break;
    case BOOK2:fileName = "book02"; break;
    case BOOK3:fileName = "book03"; break;
    case BOOK4:fileName = "book04"; break;
    case BOOK5:fileName = "book05"; break;
    case BOOK6:fileName = "book06"; break;

    default:break;
    }

    return directoryFilePath + "/" + fileName + ".png";
}

std::string MemoManager::GetKey(const MemoType type)
{
    std::string key;
    switch (type) {
    case MEMO1: key = "memo1";   break;
    case MEMO2: key = "memo2";  break;
    case MEMO3: key = "memo3";  break;
    case MEMO4: key = "memo4";  break;
    case MEMO5: key = "memo5";  break;
    case BOOK1: key = "book1";  break;
    case BOOK2: key = "book2";  break;
    case BOOK3: key = "book3";  break;
    case BOOK4: key = "book4";  break;
    case BOOK5: key = "book5";  break;
    case BOOK6: key = "book6";  break;
    case TEXT1: key = "text1"; break;
    default: break;
    }
    return key;

}

MemoManager::MemoManager()
{
    sprite_ = std::make_unique<Sprite>();
    float width = static_cast<float>(Window::GetClientWidth());
    float height = static_cast<float>(Window::GetClientHeight());
    sprite_->Create(TextureFactory::UV_CHECKER, { width * 0.5f, height * 0.5f });
    sprite_->SetAnchorPoint({ 0.5f,0.5f });

    //メモの作成
    uint32_t meiryob = FreeTypeManager::CreateFace("Resource/Fonts/meiryob.ttc", 0);
    text_.Initialize(meiryob);
    text_.SetString(U"");
    text_.SetPosition({ width * 0.5f, height * 0.5f });
    text_.SetColor({ 0, 0, 0, 1 });
    text_.SetAlign(Text::TextAlign::Center);
    text_.SetBlendMode(BlendMode::kBlendModeNormal);


}

MemoManager::~MemoManager()
{
}

void MemoManager::Initialize()
{
    isLookItem_ = false;
}

void MemoManager::Update()
{


    for (auto& [handle, memo] : memos_) {
        memo->Update();
    }

    if (isLookItem_) {

        Vector2 controllerPos = { 0.0f, 0.0f };
        if (Input::IsControllerStickPosMove(BUTTON_RIGHT, 0, &controllerPos)) {
        } else {
            controllerPos.y += Input::GetMouseWheel();
        }

        controllerPos.y *= TimeManager::DeltaTime();
        spriteScale_ += { controllerPos.y, controllerPos.y };
        spriteScale_.x = std::clamp(spriteScale_.x, minSpriteScale_, maxSpriteScale_);
        spriteScale_.y = std::clamp(spriteScale_.y, minSpriteScale_, maxSpriteScale_);

        sprite_->SetScale(spriteScale_);
    }
}

void MemoManager::Draw()
{
    for (auto& [handle, memo] : memos_) {
        memo->Draw();
    }
}

void MemoManager::DrawUI()
{
    //アイテムを見ていたら表示する
    if (isLookItem_) {
        Sprite::PreDraw();
        sprite_->Draw();
        //テキストがあったら表示
        text_.Draw();
    }
}

void MemoManager::GenerateMemos(const std::vector<MemoType>& types)
{
    memos_.clear(); // 既存のメモをクリア


    Json file = JsonFile::GetJsonFiles("memo");

    std::unordered_map<std::string, WorldTransform> memoTransforms;

    for (const auto& type : types) {
        auto memo = std::make_unique<Memo>();

        std::string key = GetKey(type);
        if (key.empty()) {
            continue;
        }

        uint32_t handle = Texture::AddTextureHandle(GetFilePath(type));
        memo->SetTexture(handle);
        
        if (file.contains(key)) {
            EulerTransform transform = JsonFile::JsonToEulerTransform(file[key]["transform"]);
            memo->GetWorldTransform().eTransform_ = transform;

            AABB aabb = { JsonFile::JsonToVector3(file[key]["AABB"]["min"]),JsonFile::JsonToVector3(file[key]["AABB"]["max"]) };
            memo->SetMemoSize(aabb);

            if (file[key].contains("string")) {
                std::string u8str = file[key]["string"].get<std::string>();

                std::u32string u32str;
                utf8::utf8to32(u8str.begin(), u8str.end(), std::back_inserter(u32str));

                memo->SetString(u32str);
            }
        }

        memos_[type] = std::move(memo);
    }
}

void MemoManager::SaveMemos()
{
    // 既存のJSONファイルを読み込むか、新規のJsonオブジェクトを作成
    Json& file = JsonFile::GetJsonFiles("memo");

    file.clear();

    // 2. 各Memoデータの保存
    for (const auto& [type, memo] : memos_) {
        if (!memo) continue;

        std::string key = GetKey(type);
        if (key.empty()) {
            continue;
        }

        // Transform（位置・回転）の保存
        const auto& transform = memo->GetWorldTransform().eTransform_;
        file[key]["transform"] = JsonFile::EulerTransformToJson(transform);

        // サイズキーの保存（Memoクラスが保持しているサイズキー文字列を取得）
        const AABB aabb = memo->GetMemoSize();

        file[key]["AABB"]["min"] = JsonFile::Vector3ToJson(aabb.min);
        file[key]["AABB"]["max"] = JsonFile::Vector3ToJson(aabb.max);

        // テキスト文字列の保存（存在する場合）
        if (!memo->GetString().empty()) {
            // UTF-8 文字列の受け皿を用意
            std::string u8str;
            std::u32string string = memo->GetString();
            // UTF-32 -> UTF-8 変換
            utf8::utf32to8(string.begin(), string.end(), std::back_inserter(u8str));
            // JSONへ設定
            file[key]["string"] = u8str;
        }
    }

    // 3. JSONファイルへ書き出し
    JsonFile::SaveJson("memo");
}

void MemoManager::SetSpriteSize(const MemoType& type)
{
    uint32_t handle = Texture::LoadAndGetIndex(GetFilePath(type));
    sprite_->SetSRVTextureHandle(handle);
    sprite_->AdjustTextureSize();
}

void MemoManager::RayCastHit(RaySprite& raySprite)
{
    if (isLookItem_) {
        return;
    }

    for (auto& [handle, memo] : memos_) {

        if (raySprite.Intersect(memo.get())) {

            memo->SetColor({ 1.0f,0.0f,0.0f,1.0f });

            if (InputBind::IsClick()) {
                if (!isLookItem_) {
                    isLookItem_ = true;
                    SetSpriteSize(handle);
                    spriteScale_ = { minSpriteScale_ ,minSpriteScale_ };
                    Sound::PlayOriginSE(SoundFactory::BOOK);

                    text_.SetString(memo->GetString());

                    return;
                }

            }

        }
    }
}

void MemoManager::GenerateAllMemo()
{
    GenerateMemos({
        MEMO1,
        MEMO2,
        MEMO3,
        MEMO4,
        MEMO5,
        BOOK1,
        BOOK2,
        BOOK3,
        BOOK4,
        BOOK5,
        BOOK6,
        TEXT1 });
}

void MemoManager::Debug()
{
#ifdef USE_IMGUI
    ImGui::Begin("MemoManager");

    if (ImGui::Button("Save")) {
        SaveMemos();
    }
    for (auto& [handle, memo] : memos_) {
        ImGui::PushID(static_cast<int>(handle));
        if (ImGui::TreeNode(("Memo"+ std::to_string(handle)).c_str())){
            DebugUI::CheckWorldTransform(memo->GetWorldTransform(), "worldTransform");

            std::u32string string = memo->GetString();
            std::string u8str;
            utf8::utf32to8(string.begin(), string.end(), std::back_inserter(u8str));
   
            static char textBuffer[256] = "";
            strncpy_s(textBuffer, sizeof(textBuffer), u8str.c_str(), _TRUNCATE);
           
            
            // 3. ImGui で入力（変更があれば true が返る）
            if (ImGui::InputText("setString", textBuffer, sizeof(textBuffer))) {
                // 4. 入力された char 配列を UTF-8 (std::string) に変換
                std::string inputU8 = textBuffer;

                // 5. UTF-8 -> UTF-32 に変換して Memo にセット
                std::u32string inputU32;
                utf8::utf8to32(inputU8.begin(), inputU8.end(), std::back_inserter(inputU32));

                memo->SetString(inputU32);
            }

           

            ImGui::TreePop();
        }
    

        ImGui::PopID();
    }
    ImGui::End();
#endif

}
