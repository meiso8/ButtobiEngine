#include "SlidePuzzleSystem.h"
#include"SoundManager/SoundManager.h"
#include<algorithm>
#include"CollisionManager.h"
#include"InputBind.h"
#include"TimeManager.h"
#include"FlagManager/FlagManager.h"

 Puzzle* SlidePuzzleSystem::currentPuzzle_ =nullptr;

SlidePuzzleSystem::SlidePuzzleSystem(const int horizontal, const int vertical, TextureFactory::Handle handle)
{
    puzzle_ = std::make_unique<Puzzle>(horizontal, vertical);
    puzzle_->SetTexture(handle);
    puzzleObj_ = std::make_unique<PuzzleObj>();
}

SlidePuzzleSystem::~SlidePuzzleSystem()
{
    puzzle_.reset();
    puzzleObj_.reset();
    currentPuzzle_ = nullptr;
}

void SlidePuzzleSystem::Initialize(const Vector3& pos,const float& rotY)
{
    FlagManager::SetFlag("SlidePuzzleIsActive",false);
    isActive_ = false;
    isEnd_ = false;
    puzzle_->Init();
    puzzleObj_->Initialize(pos, rotY);

}

void SlidePuzzleSystem::InitStaticActiveFlag()
{
    FlagManager::SetFlag("SlidePuzzleIsActive", false);
}

void SlidePuzzleSystem::SetCurrentPtr()
{

    //最近のパズルを入れる
    currentPuzzle_ = puzzle_.get();
}

void SlidePuzzleSystem::Update(const Vector2& screenPos)
{



    //クリアしたら
    if (currentPuzzle_->IsClear()) {
        if (!isEnd_) {
            SoundManager::PlayCorrectSE();
            //開ける
            puzzleObj_->SetIsOpen(true);
            //開けたとき一律でアクティブフラグを下げる
            FlagManager::SetFlag("SlidePuzzleIsActive", false);
            isActive_ = false;
            isEnd_ = true;
        }
    }

    if (!FlagManager::HasFlag("SlidePuzzleIsActive")) {
        //一律でフラグがfalseな時はアクティブをfalseにする
        isActive_ = false;
    }

    if (isActive_) {
        currentPuzzle_->Game(screenPos);
    }

    puzzleObj_->Update();

#ifdef USE_IMGUI

    //デバック用に一旦
    if (Input::IsTriggerKey(DIK_P)) {
        isEnd_ = true;
    }

    ImGui::Begin("Game");
    ImGui::Checkbox("isEndPuzzle", &isEnd_);
    ImGui::End();
#endif //USE_IMGUI

}

void SlidePuzzleSystem::Draw()
{
    puzzleObj_->Draw();
}

void SlidePuzzleSystem::DrawUI()
{
    if (isActive_) {
        currentPuzzle_->Draw();
    }
}

void SlidePuzzleSystem::RayCastHit(RaySprite& ray)
{
    //アクティブならヒットさせない
    if (isActive_) {
        return;
    }

    if (ray.Intersect(puzzleObj_.get())) {

        puzzleObj_->SetColor({ 1.0f,0.0f,0.0f,1.0f });

        if (InputBind::IsClick()) {
            if (!isActive_) {
                isActive_ = true;
                //一律でスライドパズルをアクティブにする
                FlagManager::SetFlag("SlidePuzzleIsActive", true);
            }
        }
    }

}


