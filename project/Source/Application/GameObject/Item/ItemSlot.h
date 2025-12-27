#pragma once

#include"Object3d.h"
#include<list>
#include<map>
#include"ModelManager.h"
#include"Sprite.h"
#include<memory>
#include"Collider.h"
class Item :public Collider{
public:
    Item();
    virtual ~Item() = default;
    virtual void SetModel(const ModelManager::MODEL_HANDLE& handle);
    // 3Dオブジェクトの参照（描画や当たり判定用）
    std::shared_ptr<Object3d> object_;
    // アイテムの名前や説明 
    std::string name_;
    std::string description_;
    // 使用時の効果（プレイヤーや環境に作用）
    virtual void Use() = 0;
    // 他のアイテムと組み合わせたときの処理 

    virtual std::shared_ptr<Item> CombineWith(const std::shared_ptr<Item>& other) {
        return nullptr; // デフォルトは組み合わせ不可 
    }

    // UI表示用（ImGuiなど） 
    virtual void Init();
    virtual void DrawInfoUI();
    virtual void Update();
    virtual void Draw(Camera& camera);

    void OnCollision(Collider* collider)override;
    Vector3 GetWorldPosition() const {
        return object_->worldTransform_.GetWorldPosition();
    };



};

// ====================================================================
class ItemSlot {
public:
    ItemSlot();
    static const int kMaxSlots_ = 8;
    std::array<std::shared_ptr<Item>, kMaxSlots_> slots_;
    std::array<std::unique_ptr<Sprite>, kMaxSlots_> slotSprites_;
    void OnTriggerItemPickup(const std::shared_ptr<Item>& item);
    void Update();
    bool AddItem(const std::shared_ptr<Item>& item);
    void UseItem(int index);
    void CombineItems(int indexA, int indexB);
    void DrawInfoUI();
    void Draw(Camera& camera);
};
