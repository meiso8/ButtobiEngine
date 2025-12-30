#pragma once
#include "ItemSlot.h"
#include "CrowbarItem.h"
#include "SunMedal.h"
#include "GoldHeart.h"
#include"Line.h"
class RaySprite;

class ItemManager {
public:
    ItemManager();
    void Init();
    void Update();
    void Draw(Camera& camera);
    void DrawInfoUI();

    std::shared_ptr<Item> GetItem(const std::string& name);

    // プレイヤーのアイテムスロットにアクセス
    ItemSlot& GetItemSlot() { return itemSlot_; }

    // 指定アイテムをスロットに追加（名前で）
    bool AddItemToSlot(const std::string& name);
    std::shared_ptr<Item> RaycastHitItem(RaySprite& raySprite);
    void UseItemFromSlot(const Vector3& pos);
private:
    std::map<std::string, std::shared_ptr<Item>> items_;
    ItemSlot itemSlot_;
};
