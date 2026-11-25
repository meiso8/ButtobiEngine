#include "PianoBlockMap.h"
#include"SoundManager/SoundManager.h"
#include"CollisionManager.h"
#include"Sound.h"
#include"ObjectManager/ObjectManager.h"

namespace {
    constexpr float kWhiteEndOffset = -0.5f;
    constexpr float kBlackEndOffset = -0.25f;
}

PianoBlockMap::PianoBlockMap()
{

    AABB blackAABB = { .min = {-0.5f, -0.5f, -1.5f}, .max = {0.5f, 0.5f, 1.5f} };
    blackAABB.min.x *= 0.5f;
    blackAABB.max.x *= 0.5f;
    AABB collisionBlackAABB = { blackAABB.min * 0.75f,blackAABB.max * 0.75f };

    AABB aabb = { .min = {-0.5f, -0.5f, -2.0f}, .max = {0.5f, 0.5f, 2.0f} };
    AABB collisionAABB = { aabb.min * 0.75f,aabb.max * 0.75f };


    for (auto& block : blocks_) {
        block = std::make_unique<Block>();
        block->SetTextureHandle(TextureFactory::NONE);
    }

    for (int i = 0; i < kMaxHz; ++i) {
        bool isBkack = IsBlack(i);
        blocks_[i]->SetAABB(isBkack? collisionBlackAABB :collisionAABB);
        blocks_[i]->SetCubeSize(isBkack ? blackAABB: aabb);
        blocks_[i]->SetColor(isBkack ? COLOR::BLACK : COLOR::WHITE);
        blocks_[i]->SetUseOutline(true);

    }

    whiteMap_[0] = blocks_[kC].get();
    whiteMap_[1] = blocks_[kD].get();
    whiteMap_[2] = blocks_[kE].get();
    whiteMap_[3] = blocks_[kF].get();
    whiteMap_[4] = blocks_[kG].get();
    whiteMap_[5] = blocks_[kA].get();
    whiteMap_[6] = blocks_[kB].get();
    whiteMap_[7] = blocks_[kC_H].get();

    blackMap_[0] = blocks_[kC_S].get();
    blackMap_[1] = blocks_[kD_S].get();
    blackMap_[2] = blocks_[kF_S].get();
    blackMap_[3] = blocks_[kG_S].get();
    blackMap_[4] = blocks_[kA_S].get();
}

void PianoBlockMap::Initialize()
{

    steppedOrder_.clear();

    isClear_ = false;

    for (auto& isPush : isPushFlags_) {
        isPush = false;
    }

    for (auto& block : blocks_) {
        block->Initialize();
    }

    AABB collisionAABB = whiteMap_[0]->GetAABB();
    collisionAABB.min *= 0.666f;
    collisionAABB.max *= 0.666f;
    float blockSize = collisionAABB.max.x - collisionAABB.min.x;

    float offset = -blockSize * 2.0f * 4.0f + blockSize;

    for (int i = 0; i < whiteMap_.size(); ++i) {
        Vector3 pos = {
            static_cast<float>(i) * blockSize * 2.0f + offset,
            0.25f,
            6.5f
        };

        whiteMap_[i]->SetPos(pos, kWhiteEndOffset);
        whiteMap_[i]->SetTemperature(0.025f * (i + 1));
    }

    AABB collisionBlackAABB = blackMap_[0]->GetAABB();
    collisionBlackAABB.min *= 0.666f;
    collisionBlackAABB.max *= 0.666f;
    float blockSizeBlack = collisionBlackAABB.max.x - collisionBlackAABB.min.x;

    for (int i = 0; i < blackMap_.size(); ++i) {

        float posX = 0.0f;
        if (i >= 2) {
            posX = whiteMap_[i + 1]->GetPos().x;

        } else {
            posX = whiteMap_[i]->GetPos().x;
        }

        posX += blockSizeBlack * 2.0f;

        Vector3 pos = {
          posX,
           0.75f,
           7.0f
        };

        blackMap_[i]->SetPos(pos, kBlackEndOffset);
        blackMap_[i]->SetTemperature(0.3f);
    }

}

void PianoBlockMap::Register()
{
    int i = 0;
    for (auto& block : blocks_) {
        block->RegisterObject("PianoBlock:" + std::to_string(i++));
    }
}

void PianoBlockMap::Update()
{
    for (int i = 0; i < kMaxHz; ++i) {
        blocks_[i]->Update();
        Vector4 color = IsBlack(i) ? COLOR::BLACK : COLOR::WHITE;
        blocks_[i]->SetColor(color);
    }

#ifdef USE_IMGUI

    ImGui::Begin("Game");
    if (ImGui::Button("BastetBlockClear")) {
        ClearSet();
    }
    ImGui::End();
#endif
    if (isClear_) {
        NormalPiano();
    } else {
        PuzzlePiano();
    }


}

void PianoBlockMap::Draw()
{
    for (auto& block : blocks_) {
        block->Draw();
    }
}

void PianoBlockMap::ResetPushMap()
{
    for (int i = 0; i < kMaxHz; ++i) {
        if (blocks_[i]->GetIsPush()) {
            blocks_[i]->Reset(false);
            //戻す位置を設定
            blocks_[i]->SetEndPos(IsBlack(i) ? kBlackEndOffset : kWhiteEndOffset);
        }
    }

}

void PianoBlockMap::RayCastHit(RaySprite& raySprite)
{
    for (int i = 0; i < kMaxHz; ++i) {

        if (!blocks_[i]->GetIsPush()) {

            if (blocks_[i]->GetObjectId() == ObjectManager::GetInstance()->GetHoverID() || raySprite.Intersect(blocks_[i].get(), 10.0f)) {
                Vector4 color{};
                if (IsBlack(i)) {
                    color = { 0.25f, 0.25f, 0.25f, 1.0f };
                } else {
                    color = COLOR::ToShadowColor(COLOR::WHITE);
                }


                blocks_[i]->SetColor(color);
                //ブロックテクスチャによって判定しない
                blocks_[i]->RayCastHit(false);
                raySprite.CanSelect(blocks_[i].get(), 10.0f);
                break;
            }
        }
    }
}

void PianoBlockMap::ClearSet()
{
    isClear_ = true;
    SoundManager::PlayCorrectSE();
    SoundManager::PlayGOGOGOSE();
}

void PianoBlockMap::NormalPiano()
{

    for (int i = 0; i < kMaxHz; ++i) {

        if (blocks_[i]->GetIsPush()) {
            PianoPushSound(i);
        }

        if (isPushFlags_[i]) {
            //プッシュしていたら
            //フラグも戻す
            isPushFlags_[i] = false;

            blocks_[i]->Reset(false);
            //戻す位置を設定
            blocks_[i]->SetEndPos(IsBlack(i) ? kBlackEndOffset : kWhiteEndOffset);

        }
    }

}

void PianoBlockMap::PuzzlePiano()
{
    bool isReset = false;

    for (int i = 0; i < kMaxHz; ++i) {
        if (blocks_[i]->GetIsPush()) {

            PianoPushSound(i);

            // すでに踏んだ順番に追加（重複防止）
            if (std::find(steppedOrder_.begin(), steppedOrder_.end(), i) == steppedOrder_.end()) {
                steppedOrder_.push_back(i);
            }

            // 間違った順番ならリセット
            if (steppedOrder_.size() >= correctOrder_.size() ||
                steppedOrder_[steppedOrder_.size() - 1] != correctOrder_[steppedOrder_.size() - 1]) {

                isReset = true;
            }

            // 正しい順番と一致したらクリア
            if (steppedOrder_ == correctOrder_) {
                if (!isClear_) {
                    ClearSet();
                    ResetPushMap();
                    return;
                }
            }

        }
    }


    if (isReset) {

        for (auto& isPush : isPushFlags_) {
            isPush = false;
        }
        steppedOrder_.clear();
        SoundManager::PlayCancelSE();
        ResetPushMap();
    }
}

void PianoBlockMap::PianoPushSound(const int index)
{

    if (!isPushFlags_[index]) {
        isPushFlags_[index] = true;
        //どの音からピッチを変化させてみる
        Sound::PlaySE(SoundFactory::Sound_C, 2.0f);
        Sound::SetFrequencyRatio(SoundFactory::Sound_C, index);
    }
}

bool PianoBlockMap::IsBlack(const int index)
{
    return kC_S == index || kD_S == index || kF_S == index || kG_S == index || kA_S == index;
}
