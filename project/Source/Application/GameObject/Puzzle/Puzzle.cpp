#include "Puzzle.h"
#include"Input.h"
#define rep(i,n)for(int i =0;i < n;++i)
using namespace std;
#include"DebugUI.h"
#include"Collision.h"
#include"Sound.h"
#include"SoundManager/SoundManager.h"
#include"InputBind.h"

void Puzzle::Change(int x, int y)
{
    int p1 = y * verticalCount_ + x;
    int p2 = -1;
    int maxIndex = maxArrayNum_ - 1;

    if (x > 0 && panel_[p1 - 1] == maxIndex) p2 = p1 - 1;
    if (x < horizontalCount_ - 1 && panel_[p1 + 1] == maxIndex) p2 = p1 + 1;
    if (y > 0 && panel_[p1 - horizontalCount_] == maxIndex) p2 = p1 - horizontalCount_;
    if (y < verticalCount_ - 1 && panel_[p1 + horizontalCount_] == maxIndex) p2 = p1 + horizontalCount_;

    if (p2 != -1) {
        //入れ替わったら行われる
        panel_[p2] = panel_[p1];
        panel_[p1] = maxIndex;
    }


}

Vector2 Puzzle::CheckMove(const Vector2 pos)
{

    int tempIndex = curentHoverIndex_;

    if (tempIndex % horizontalCount_ < horizontalCount_ - 1) {
        //右に移動で可能
        if (InputBind::IsTriggerMoveR()) {
            tempIndex += 1;
        }
    }

    if (tempIndex % horizontalCount_ > 0) {
        //左に移動可能
        if (InputBind::IsTriggerMoveL()) {
            tempIndex -= 1;
        }
    }

    if (tempIndex / horizontalCount_ > 0) {
        //上に移動可能

        if (InputBind::IsTriggerMoveF()) {
            tempIndex -= horizontalCount_;
        }
    }

    if (tempIndex / horizontalCount_ < verticalCount_ - 1) {
        //下に移動可能
        if (InputBind::IsTriggerMoveB()) {
            tempIndex += horizontalCount_;
        }
    }

    if (InputBind::IsTriggerMoveB() || InputBind::IsTriggerMoveF() || InputBind::IsTriggerMoveL() || InputBind::IsTriggerMoveR()) {
        if (panel_[tempIndex] < panel_.size() - 1) {
            curentHoverIndex_ = tempIndex;
            return  sprites_[panel_[curentHoverIndex_]]->GetPosition();
        }
    }
    //元のポジション
    return pos;
}



Puzzle::Puzzle(const int horizontal, const int vertical, const TextureFactory::Handle& handle)
{
    horizontalCount_ = horizontal;
    verticalCount_ = vertical;
    maxArrayNum_ = verticalCount_ * horizontalCount_;

    centerPos_ = { float(Window::GetClientWidth() * 0.5f),float(Window::GetClientHeight() * 0.5f) };

    sprite_ = make_unique<Sprite>();
    sprite_->Create(handle, centerPos_);
    sprite_->SetAnchorPoint({ 0.5f,0.5f });

    Vector2 size = sprite_->GetSize();
    size_ = { size.x / horizontalCount_,size.y / verticalCount_ };

    centerPos_ -= size_ * static_cast<float>(horizontalCount_) * 0.375f;

    sprites_.resize(maxArrayNum_);
    panel_.resize(maxArrayNum_);

    rep(i, maxArrayNum_) sprites_[i] = make_unique<Sprite>(),
        sprites_[i]->Create(handle, { 0.0f,0.0f });

    for (int i = 0; i < maxArrayNum_; ++i) {
        int col = i % horizontalCount_; // 横方向のインデックス（0〜3）
        int row = i / horizontalCount_; // 縦方向のインデックス（0〜3）
        sprites_[i]->SetSize(size_);
        sprites_[i]->SetTextureSize(size_);
        sprites_[i]->SetTextureLeftTop({ col * size_.x, row * size_.y });
        sprites_[i]->SetAnchorPoint({ 0.5f,0.5f });
        sprites_[i]->SetPosition({ centerPos_.x + col * size_.x,centerPos_.y + row * size_.y });
    }

    frameSprite_ = std::make_unique<Sprite>();
    frameSprite_->Create(TextureFactory::PUZZLE_FRAME, centerPos_);
    frameSprite_->SetAnchorPoint({ 0.5f,0.5f });
    frameSprite_->SetSize(size_ + Vector2{8.0f,8.0f});
    curentHoverIndex_ = -1;
}

Puzzle::~Puzzle()
{
}

void Puzzle::Init()
{
    isClear_ = false;
    Shuffle();

}

void Puzzle::Shuffle()
{
    rep(i, maxArrayNum_) panel_[i] = i;
    rep(j, 1000) Change(rand() % horizontalCount_, rand() % verticalCount_);
    rep(j, 1000) Change(random.Get() % horizontalCount_, random.Get() % verticalCount_);
}

void Puzzle::Game(const Vector2& screenPos)
{

    if (isClear_) {
        return;
    }

    int clickedIndex = -1;

    for (int i = 0; i < maxArrayNum_; ++i) {

        Sprite* sprite = sprites_[panel_[i]].get();

        if (IsCollision(screenPos, *sprite)) {

            curentHoverIndex_ = i;
            
            //フレーム位置を設定する
            frameSprite_->SetPosition(sprites_[panel_[curentHoverIndex_]]->GetPosition());

            if (InputBind::IsClick()) {
                clickedIndex = i;
                Sound::PlaySE(SoundFactory::MOVE_ROCK, 2.0f);
            }

        } 
    }

    // クリックされたパネルがあれば処理
    if (clickedIndex != -1) {
        int x = clickedIndex % horizontalCount_;
        int y = clickedIndex / horizontalCount_;
        Change(x, y);

        // クリア判定
        isClear_ = true;
        rep(i, maxArrayNum_) if (panel_[i] != i) isClear_ = false;
    }

    for (int i = 0; i < maxArrayNum_; ++i) {
        if (panel_[i] < maxArrayNum_ - 1) {
            int col = i % horizontalCount_; // 横方向のインデックス（0〜3）
            int row = i / horizontalCount_; // 縦方向のインデックス（0〜3）
            sprites_[panel_[i]]->SetPosition({ centerPos_.x + col * size_.x,centerPos_.y + row * size_.y });
        }
    }
}


void Puzzle::Draw()
{
    Sprite::PreDraw();

    if (isClear_) {
        sprite_->Draw();
    } else {
        rep(i, maxArrayNum_)   if (panel_[i] < maxArrayNum_ - 1) {
            sprites_[panel_[i]]->Draw();
        }


        if (curentHoverIndex_ != -1) {
            frameSprite_->Draw();
        }
    }


}
