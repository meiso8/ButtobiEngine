#include "Block.h"
#include"Easing.h"
#include"Window.h"
#include<algorithm>
#include"SoundManager/SoundManager.h"
#include"Sound.h"

void Block::Initialize()
{
    object_->Initialize();
    aniTimer_ = 0.0f;
    isPush_ = false;
    isHit_ = false;
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

    if (isHit_) {
        return;
    }
    // 通常ブロックなら無視 
    if (cubeMesh_->GetSrvIndex() == Texture::GetHandle(Texture::PUZZLE)|| cubeMesh_->GetSrvIndex() == Texture::GetHandle(Texture::NONE)) {
        return;
    };

    Sound::PlaySE(Sound::MOVE_ROCK,0.5f);
    aniTimer_ = 0.0f;
    isHit_ = true;

}

void Block::SetPos(const Vector3& pos, const float& endOffset)
{
    object_->worldTransform_.translate_ = pos;
    startPosY_ = pos.y;
    SetEndPos(endOffset);
}

void Block::SetEndPos(const float& endOffset)
{
    endPosY_ = startPosY_ + endOffset;
}

void Block::Reset()
{
    // 通常ブロックなら無視 
    if (cubeMesh_->GetSrvIndex() == Texture::GetHandle(Texture::PUZZLE)|| cubeMesh_->GetSrvIndex() == Texture::GetHandle(Texture::NONE)) {
        return;
    };

    aniTimer_ = 0.0f;
    isPush_ = false;
    isHit_ = false;
}

BlockMap::BlockMap()
{
    AABB aabb = { .min = {-1.5f, -1.5f, -1.5f}, .max = {1.5f, 1.5f, 1.5f} };
    float blockSize = aabb.max.x - aabb.min.x; // = 3.0f
    float offsetX = -(kMapWidth * blockSize) * 0.5f + aabb.max.x;
    float offsetZ = -(kMapHeight * blockSize) * 0.5f + aabb.max.y;

    for (int y = 0; y < kMapHeight; ++y) {
        for (int x = 0; x < kMapWidth; ++x) {
            map_[y][x] = std::make_unique<Block>();
            map_[y][x]->Initialize();

            Vector3 pos = {
                static_cast<float>(x) * blockSize + offsetX,
                -1.5f,
                static_cast<float>(y) * blockSize + offsetZ
            };

            map_[y][x]->SetPos(pos);
            map_[y][x]->SetCubeAABB(aabb);
            if (rand() % 2 == 0) {
                map_[y][x]->SetTextureHandle(Texture::PUZZLE);
            } else {
                map_[y][x]->SetTextureHandle(Texture::NONE);
            }
       
        }
    }



    // ヒエログリフ付きブロックを配置（例：中央付近）
    map_[4][2]->SetTextureHandle(Texture::HIERO_S);
    map_[1][5]->SetTextureHandle(Texture::HIERO_T);
    map_[2][1]->SetTextureHandle(Texture::HIERO_D);
    map_[3][3]->SetTextureHandle(Texture::HIERO_P);
    Vector4 color = { 1.0f,1.0f,0.0f,1.0f };
    map_[4][2]->SetColor(color);
    map_[1][5]->SetColor(color);
    map_[2][1]->SetColor(color);
    map_[3][3]->SetColor(color);

    centerBlocks_[0] = map_[3][3].get(); // 左上
    centerBlocks_[1] = map_[4][3].get(); // 右上
    centerBlocks_[2] = map_[3][4].get(); // 左下
    centerBlocks_[3] = map_[4][4].get(); // 右下

}

void BlockMap::Initialize() {

    isClear_ = false;
    steppedOrder_.clear();
}

void BlockMap::Update() {

    for (auto& y : map_) {
        for (auto& block : y) {
            block->Update();
        }
    }

    if (isClear_) {
        return;
    }


    // ======================================================================================
    bool isReset = false;

    for (auto& y : map_) {
        for (auto& block : y) {

            if (block->IsHit()) {
                //まだ踏んでないトキ且つヒットしたとき
                Texture::TEXTURE_HANDLE tex = Texture::GetTextureHandle(block->GetSrvIndex());

                // すでに踏んだ順番に追加（重複防止）
                if (std::find(steppedOrder_.begin(), steppedOrder_.end(), tex) == steppedOrder_.end()) {
                    block->SetIsPush(true);
                    steppedOrder_.push_back(tex);
                }

                // 正しい順番と一致したら水を引く！
                if (steppedOrder_ == correctOrder_) {
                    if (!isClear_) {
                        isClear_ = true;
                        SoundManager::PlayCorrectSE();
                        SoundManager::PlayGOGOGOSE();
                        for (auto& block : centerBlocks_) { if (block) {
                            block->SetEndPos(-1.5f);
                            block->SetIsPush(true); 
                        } }
                        return;
                    }
                }

                // 間違った順番ならリセット
                if (steppedOrder_.size() >= correctOrder_.size() ||
                    steppedOrder_[steppedOrder_.size() - 1] != correctOrder_[steppedOrder_.size() - 1]) {
                    steppedOrder_.clear();
                    isReset = true;
                }


            }

        }
    }


    if (isReset) {
        SoundManager::PlayCancelSE();
        for (auto& y : map_) {
            for (auto& block : y) {
                block->Reset();
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
