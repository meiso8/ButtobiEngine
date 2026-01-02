#pragma once
#include<array>
#include<memory>
#include"Sprite.h"
class Puzzle
{
private:
    //水平
    static const int horizontal_ = 4;
    //垂直
    static const int vertical_ = 4;
    static const int maxArrayNum_ = vertical_ * horizontal_;
    std::array<int, maxArrayNum_>panel_;
    //画像
    std::array< std::unique_ptr<Sprite>, maxArrayNum_>sprites_;
    //画像全て
    std::unique_ptr<Sprite>  sprite_ = nullptr;
    bool isClear_ = false;
    Vector2 size_ = { 0.0f };
    Vector2 centerPos_ = { 0.0 };


private:
    //シャッフル
    void Shuffle();
    //選択したパネルと空白を切り替える
    void Change(int x, int y);
public:
    Puzzle();
    void Init();
    void Game();
    void Update();
    void Draw();
    bool IsClear() { return isClear_; };

    void SetTexture(const Texture::TEXTURE_HANDLE& handle);
};

