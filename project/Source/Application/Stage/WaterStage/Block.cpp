#include "Block.h"
#include"Easing.h"
#include"Window.h"
#include<algorithm>
#include"SoundManager/SoundManager.h"
#include"Sound.h"

void Block::Initialize()
{
    assert(object_);

    object_->Initialize();
    aniTimer_ = 0.0f;
    isPush_ = false;
}

void Block::Update()
{
    aniTimer_ += InverseFPS;
    aniTimer_ = std::clamp(aniTimer_, 0.0f, 1.0f);

    if (isPush_) {
        object_->worldTransform_.translate_.y = Easing::EaseInOut(startPosY_, endPosY_, aniTimer_);
    } else {
        object_->worldTransform_.translate_.y = Easing::EaseInOut(endPosY_, startPosY_, aniTimer_);
    }

    object_->Update();
}

void Block::OnCollision(Collider* collider)
{

    if (isPush_) {
        return;
    }

    Sound::PlaySE(Sound::THROW);
    isPush_ = true;
    aniTimer_ = 0.0f;

}

BlockMap::BlockMap()
{
    AABB aabb = { .min = {-1.5f, -1.5f, -1.5f}, .max = {1.5f, 1.5f, 1.5f} };
    float blockSize = aabb.max.x - aabb.min.x; // = 3.0f
    float offsetX = -(kMapWidth * blockSize) * 0.5f;
    float offsetZ = -(kMapHeight * blockSize) * 0.5f;

    for (int y = 0; y < kMapHeight; ++y) {
        for (int x = 0; x < kMapWidth; ++x) {
            map_[y][x] = std::make_unique<Block>();
            map_[y][x]->Initialize();

            Vector3 pos = {
                static_cast<float>(x) * blockSize +offsetX,
                -3.0f,
                static_cast<float>(y) * blockSize + offsetZ
            };

            map_[y][x]->SetPos(pos);
            map_[y][x]->SetCubeAABB(aabb);
            map_[y][x]->SetTextureHandle(Texture::PUZZLE);
        }
    }


    // ヒエログリフ付きブロックを配置（例：中央付近）
    map_[3][2]->SetTextureHandle(Texture::HIERO_S);
    map_[3][3]->SetTextureHandle(Texture::HIERO_T);
    map_[3][4]->SetTextureHandle(Texture::HIERO_D);
    map_[3][5]->SetTextureHandle(Texture::HIERO_P);
}

void BlockMap::Initialize() {

    isClear_ = false;
    steppedOrder_.clear();
}

void BlockMap::Update() {


    for (auto& y : map_) {
        for (auto& block : y) {

            block->Update();

            if (block->IsHit() && block->GetIsPush()) {
                Texture::TEXTURE_HANDLE tex = block->GetTextureHandle();

                // すでに踏んだ順番に追加（重複防止）
                if (std::find(steppedOrder_.begin(), steppedOrder_.end(), tex) == steppedOrder_.end()) {
                    steppedOrder_.push_back(tex);
                }

                // 正しい順番と一致したら水を引く！
                if (steppedOrder_ == correctOrder_) {
                    if (!isClear_) {
                        isClear_ = true;
                        SoundManager::PlayCorrectSE();
                    }

                }

                // 間違った順番ならリセット
                if (steppedOrder_.size() > correctOrder_.size() ||
                    steppedOrder_[steppedOrder_.size() - 1] != correctOrder_[steppedOrder_.size() - 1]) {
                    steppedOrder_.clear();
                    SoundManager::PlayCancelSE();
                }
            }
        }
    }

}


void BlockMap::Draw(Camera& camera)
{
    for (auto& y : map_) {
        for (auto& x : y) {
            x->Draw(camera);
        }
    }
}
