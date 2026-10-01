#pragma once

#include<memory>
class Sprite;

class Skip
{

public:
    Skip();
    ~Skip();
    void Initialize();
    void Draw();
    void SetIsDraw(const bool flag) { isSkipDraw_ = flag; }
    bool GetIsSkip() { return isSkipDraw_; }
private:
    bool isSkipDraw_ = false;
    std::unique_ptr<Sprite> skipSprite_ = nullptr;
};

