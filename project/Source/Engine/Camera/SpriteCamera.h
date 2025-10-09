#pragma once
#include"Matrix4x4.h"

class SpriteCamera
{
private:
    static SpriteCamera* instance_;
    static Matrix4x4 viewProjectionMat_;

    SpriteCamera() = default;
    ~SpriteCamera() = default;
    SpriteCamera(SpriteCamera&) = delete;
    SpriteCamera& operator=(SpriteCamera&) = delete;
public:
    static SpriteCamera* GetInstance();

    void Initialize(const float& width, const float& height);
    static Matrix4x4& GetViewProjectionMatrix() {
        return viewProjectionMat_
            ;
    };

};

