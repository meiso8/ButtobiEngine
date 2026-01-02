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
public:
    static bool isLookItem_;
    MemoManager();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    void DrawUI();
    void GenerateMemos(const std::vector<Texture::TEXTURE_HANDLE>& handles);

    void SetSpriteSize(const Texture::TEXTURE_HANDLE& handle);
    void RayCastHit(RaySprite& raySprite);
};
