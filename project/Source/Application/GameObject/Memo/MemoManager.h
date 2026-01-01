#pragma once
#include"Memo.h"
#include<map>
#include"Sprite.h"
#include"Player/RaySprite.h"

class MemoManager
{
private:
    std::map< Texture::TEXTURE_HANDLE, std::unique_ptr<Memo>>memos_;
    std::unique_ptr<Sprite> sprite_;
    std::unique_ptr<Sprite> backSprite_;
public:
    static bool isLookItem_;
    MemoManager();
    void Initialize();
    void Update();
    void Draw(Camera& camera);

    void SetSpriteSize(const Texture::TEXTURE_HANDLE& handle);
    void RayCastHit(RaySprite& raySprite);
    void LookExit();
};
