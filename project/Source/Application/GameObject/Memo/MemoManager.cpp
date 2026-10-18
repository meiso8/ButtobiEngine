#include "MemoManager.h"
#include"JsonFile/JsonFile.h"
#include"CollisionManager.h"
#include"InputBind.h"
#include"Sound.h"
#include<algorithm>
#include"TimeManager.h"

bool MemoManager::isLookItem_ = false;

namespace {
   float maxSpriteScale_ = { 2.0f};
   float minSpriteScale_ = { 1.0f};
}

std::string MemoManager::GetFilePath(const MemoType type)
{

    std::string fileName;

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

MemoManager::MemoManager()
{
    sprite_ = std::make_unique<Sprite>();
    float width = static_cast<float>(Window::GetClientWidth());
    float height = static_cast<float>(Window::GetClientHeight());
    sprite_->Create(TextureFactory::UV_CHECKER, { width * 0.5f, height * 0.5f });
    sprite_->SetAnchorPoint({ 0.5f,0.5f });

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
    }
}


void MemoManager::GenerateMemos(const std::vector<MemoType>& types)
{
    memos_.clear(); // 既存のメモをクリア

    Json file = JsonFile::GetJsonFiles("memo");

    std::unordered_map<std::string, WorldTransform> memoTransforms;
    std::unordered_map<std::string, AABB> memoAABB;

    // サイズ読み込み
    std::vector<std::string> sizeKeys = { "memoSize", "bookSize", "noteSize" };
    for (const auto& key : sizeKeys) {
        if (file.contains(key)) {
            AABB aabb;
            aabb.min.x = file[key]["min"]["x"];
            aabb.min.y = file[key]["min"]["y"];
            aabb.min.z = file[key]["min"]["z"];
            aabb.max.x = file[key]["max"]["x"];
            aabb.max.y = file[key]["max"]["y"];
            aabb.max.z = file[key]["max"]["z"];
            memoAABB[key] = aabb;
        }
    }

    for (const auto& type : types) {
        auto memo = std::make_unique<Memo>();
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
          default: continue;
        }

        uint32_t handle = Texture::AddTextureHandle(GetFilePath(type));
        memo->SetTexture(handle);

        if (file.contains(key)) {
             EulerTransform transform;
            transform.translate.x = file[key]["translate"]["x"];
            transform.translate.y = file[key]["translate"]["y"];
            transform.translate.z = file[key]["translate"]["z"];
            transform.rotate.x = file[key]["rotate"]["x"];
            transform.rotate.y = file[key]["rotate"]["y"];
            transform.rotate.z = file[key]["rotate"]["z"];
            memo->GetWorldTransform().eTransform_ = transform;  
            //大きさの設定は別で
            memo->SetCubeSize(memoAABB[file[key]["size"]]);
        }


        memos_[type] = std::move(memo);
    }
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
                    return;
                }

            }

        }
    }
}
