#pragma once
#include"../Engine/FreeTypeManager/Text.h"

#include<memory>
class Sprite;

#include"Network/Network.h"
class PasswordText
{
public:
    PasswordText();
    ~PasswordText() = default;
    void Initialize();
    void Update();
    void Draw();
    bool GetIsUnLock() { return isUnLock_; }
    static bool GetIsActive() { return isActive_; }
    static void SetIsActive(const bool isActive) { isActive_ = isActive; }
    void SetCurPosPtr(Vector2* curPosPtr) { curPosPtr_ = curPosPtr; }
private:

    //情報テキスト
    Text infoText_;
    //ロックが解除される
    bool isUnLock_ = false;
    static bool isActive_;
    //ウェブ用スプライト
    std::unique_ptr<Sprite>webSprite_ = nullptr;
    Vector2* curPosPtr_ = nullptr;

    //ネットワークシステム
    std::unique_ptr<Network>netWork_ = nullptr;
};

