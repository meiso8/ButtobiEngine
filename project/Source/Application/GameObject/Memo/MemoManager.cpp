#include "MemoManager.h"
#include"JsonFile/JsonFile.h"
#include"CollisionManager.h"
#include"InputBind.h"
#include"Sound.h"

bool MemoManager::isLookItem_ = false;

MemoManager::MemoManager()
{
    sprite_ = std::make_unique<Sprite>();
    float width = static_cast<float>(Window::GetClientWidth());
    float height = static_cast<float>(Window::GetClientHeight());
    sprite_->Create(Texture::MEMO1, { width * 0.5f, height * 0.5f });
    sprite_->SetAnchorPoint({ 0.5f,0.5f });

    Json file = JsonFile::GetJsonFiles("memo");
    std::vector<std::string> keys = { "memo1", "memo2", "memo3", "memo4", "book1","book2" };
    std::unordered_map<std::string, WorldTransform> memoTransforms;


    for (const auto& key : keys) {
        if (file.contains(key)) {
            WorldTransform worldTransform;

            worldTransform.translate_.x = file[key]["translate"]["x"];
            worldTransform.translate_.y = file[key]["translate"]["y"];
            worldTransform.translate_.z = file[key]["translate"]["z"];

            worldTransform.rotate_.x = file[key]["rotate"]["x"];
            worldTransform.rotate_.y = file[key]["rotate"]["y"];
            worldTransform.rotate_.z = file[key]["rotate"]["z"];

            memoTransforms[key] = worldTransform;
        }
    }

    std::unordered_map<std::string, AABB> memoAABB;


    std::vector<std::string> sizeKeys = { "memoSize", "bookSize" };
    for (const auto& key : sizeKeys) {
        if (file.contains(key)) {
            AABB aabb; // AABBの読み込み 
            aabb.min.x = file[key]["min"]["x"];
            aabb.min.y = file[key]["min"]["y"];
            aabb.min.z = file[key]["min"]["z"];

            aabb.max.x = file[key]["max"]["x"];
            aabb.max.y = file[key]["max"]["y"];
            aabb.max.z = file[key]["max"]["z"];
            memoAABB[key] = aabb;
        }
    }

    memos_[Texture::MEMO1] = std::make_unique<Memo>();
    memos_[Texture::MEMO2] = std::make_unique<Memo>();
    memos_[Texture::MEMO3] = std::make_unique<Memo>();
    memos_[Texture::MEMO4] = std::make_unique<Memo>();
    memos_[Texture::BOOK] = std::make_unique<Memo>();
    memos_[Texture::BOOK2] = std::make_unique<Memo>();

    for (auto& [handle, memo] : memos_) {
        memo->SetTexture(handle);
        if (handle == Texture::BOOK|| handle == Texture::BOOK2) {
            memo->SetCubeSize(memoAABB["bookSize"]);
        } else {
            memo->SetCubeSize(memoAABB["memoSize"]);
        }
    }

    memos_[Texture::MEMO1]->GetWorldTransform() = memoTransforms["memo1"];
    memos_[Texture::MEMO2]->GetWorldTransform() = memoTransforms["memo2"];
    memos_[Texture::MEMO3]->GetWorldTransform() = memoTransforms["memo3"];
    memos_[Texture::MEMO4]->GetWorldTransform() = memoTransforms["memo4"];
    memos_[Texture::BOOK]->GetWorldTransform() = memoTransforms["book1"];
    memos_[Texture::BOOK2]->GetWorldTransform() = memoTransforms["book2"];
}

void MemoManager::Initialize()
{
    isLookItem_ = false;
}

void MemoManager::Update()
{
    LookExit();

    for (auto& [handle, memo] : memos_) {
        memo->Update();
    }
}

void MemoManager::Draw(Camera& camera)
{
    for (auto& [handle, memo] : memos_) {
        memo->Draw(camera);
    }

    //アイテムを見ていたら表示する
    if (isLookItem_) {
        Sprite::PreDraw();
        sprite_->Draw();
    }
}

void MemoManager::SetSpriteSize(const Texture::TEXTURE_HANDLE& handle)
{
    sprite_->SetTexture(handle);
    sprite_->AdjustTextureSize();
}

void MemoManager::RayCastHit(RaySprite& raySprite)
{
    if (isLookItem_) {
        return;
    }

    for (auto& [handle, memo] : memos_) {
        AABB aabb = GetAABBWorldPos(memo.get());

        if (raySprite.IntersectsAABB(aabb, memo->GetWorldPosition())) {
            if (InputBind::IsClick()) {
                if (!isLookItem_) {
                    isLookItem_ = true;
                    SetSpriteSize(handle);
                    Sound::PlayOriginSE(Sound::BOOK);
                    return;
                }
           
            }

        }
    }


}

void MemoManager::LookExit()
{
    if (isLookItem_) {
        if (InputBind::IsClick()) {
            isLookItem_ = false;
        }
    }
}
