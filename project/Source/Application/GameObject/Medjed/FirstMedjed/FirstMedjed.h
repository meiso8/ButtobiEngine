#pragma once
#include"AnimationObject3d.h"
#include<memory>

class FirstMedjed
{
public:
    FirstMedjed();
    ~FirstMedjed();
    void Initialize();
    void Update();
    void Draw();

    void SetIsHide(const bool flag) { isHide_ = flag; };
    bool GetIsHide() { return isHide_; };
    void SetIsAppear(const bool flag) { isAppear_ = flag; }
    bool GetIsAppear() { return isAppear_; }
    void SetStartPosAndEndPos(const Vector3& startPos, const Vector3& endPos);
private:
    std::unique_ptr<AnimationObject3d> aniObj_ = nullptr;
    bool isHide_ = false;
    bool isAppear_ = false;
    float aniTimer_ = 0.0f;
    Vector3 startPos_ = {0.0f};
    Vector3 endPos_ = {0.0f};
};

