#pragma once
#include"Memo.h"
#include<map>
#include"Sprite.h"
#include"RaySprite/RaySprite.h"
#include"Vector2.h"

class MemoManager
{
public:

    enum MemoType {
        MEMO1,
        MEMO2,
        MEMO3,
        MEMO4,
        MEMO5,
        BOOK1,
        BOOK2,
        BOOK3,
        BOOK4,
        BOOK5,
        BOOK6,
    };

private:
    std::map< MemoType, std::unique_ptr<Memo>>memos_;

    std::unique_ptr<Sprite> sprite_ = nullptr;
    Vector2 spriteScale_ = { 1.0f,1.0f };
    static bool isLookItem_;
    const std::string directoryFilePath = "Resource/Textures";
public:

    std::string GetFilePath(const MemoType type);
public:
 
    static bool IsLookItem() { return isLookItem_; };
    static void SetLookItem(const bool flag) { isLookItem_ = flag; }
    MemoManager();
    ~MemoManager();

    void Initialize();
    void Update();
    void Draw();
    void DrawUI();
    void GenerateMemos(const std::vector<MemoType>& types);

    void SetSpriteSize(const MemoType& type);
    void RayCastHit( RaySprite& raySprite);
  const  std::map<MemoType, std::unique_ptr<Memo>>& GetMemos() { return memos_; }
};
