#pragma once
#include<vector>
#include<memory>
#include"Sprite.h"

class Puzzle
{
private:

    //水平
    int horizontalCount_ = 4;
    //垂直
    int verticalCount_ = 4;
    int maxArrayNum_ = verticalCount_ * horizontalCount_;

    std::vector<int>panel_;
    //画像
    std::vector< std::unique_ptr<Sprite>>sprites_;
    //画像全て
    std::unique_ptr<Sprite> sprite_ = nullptr;
    bool isClear_ = false;
    Vector2 size_ = { 0.0f };
    Vector2 centerPos_ = { 0.0 };
    int curentHoverIndex_ =-1;

    std::unique_ptr<Sprite>frameSprite_ = nullptr;

private:
    //シャッフル
    void Shuffle();
    //選択したパネルと空白を切り替える
    void Change(int x, int y);
 
public:
    Vector2 CheckMove(const Vector2 pos);
    Puzzle(const int horizontal = 4, const int vertical = 4, const TextureFactory::Handle& handle = TextureFactory::PUZZLE1);
    ~Puzzle();
    void Init();
    void Game(const Vector2& screenPos);
    void Draw();
    bool IsClear() { return isClear_; };
    //画像
    const std::vector< std::unique_ptr<Sprite>>& GetSprites() { return sprites_; };
};

