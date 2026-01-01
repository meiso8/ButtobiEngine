#pragma once
#include<array>
#include"../Platform.h"
#include<memory>

class Block :public Platform
{
private:
    bool isPush_ = false;
    float startPosY_ = { 0.0f };
    float endPosY_ = { 0.0f };
    float aniTimer_ = 0.0f;
    bool isHit_ = false;
public:
    void SetIsPush(const bool& isPush) { isPush_ = isPush; }
    const bool& GetIsPush() { return isPush_; };
    bool IsHit() const { return isHit_ && !isPush_ && cubeMesh_->GetSrvIndex() != Texture::GetHandle(Texture::PUZZLE)&&
    
    cubeMesh_->GetSrvIndex() != Texture::GetHandle(Texture::NONE);
    
    } // ← すでに踏んだブロックは無視！
    void Initialize()override;
    void Update()override;
    //void Draw(Camera& camera);
    void OnCollision(Collider* collider)override;
    void SetPos(const Vector3& pos, const float& endOffset = -0.5f);
    void SetEndPos(const float& endOffset);
    void Reset();
};

class BlockMap {
public:
    static const int kMapWidth = 8;
    static const int kMapHeight = 8;
    BlockMap();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    const bool& IsClear() { return isClear_; };
    std::array < std::array<std::unique_ptr<Block>, kMapWidth>, kMapHeight >& GetMap() { return map_; };
private:
    std::array<Block*, 4> centerBlocks_;

    std::array < std::array<std::unique_ptr<Block>, kMapWidth>, kMapHeight >map_;
    std::vector<Texture::TEXTURE_HANDLE> correctOrder_ = { Texture::HIERO_S, Texture::HIERO_P, Texture::HIERO_D, Texture::HIERO_T };
    std::vector<Texture::TEXTURE_HANDLE> steppedOrder_;
    bool isClear_ = false;
};