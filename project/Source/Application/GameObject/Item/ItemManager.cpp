#include "ItemManager.h"
#include"AABB.h"
#include"Line.h"
#include"Vector3.h"
#include "CollisionManager.h"
#include"Input.h"
#include"DebugUI.h"
#include"Player/RaySprite.h"
ItemManager::ItemManager()
{
 
}
void ItemManager::Init() {
    auto crowbar = std::make_shared<CrowbarItem>();
    crowbar->Init();
    items_["バールのようなもの"] = crowbar;

    auto sunMedal = std::make_shared<SunMedal>();
    sunMedal->Init();
    items_["太陽のメダル"] = sunMedal;

    auto goldHeart = std::make_shared<GoldHeart>();
    goldHeart->Init();
    items_["金の心臓"] = goldHeart;

    itemSlot_.Init();
}

void ItemManager::Update() {
    for (auto& [name, item] : items_) {
        item->Update();
    }
    itemSlot_.Update();
}

void ItemManager::Draw(Camera& camera) {
    for (auto& [name, item] : items_) {
        item->Draw(camera);
    }
    itemSlot_.Draw(camera);
}

void ItemManager::DrawInfoUI() {
    for (auto& [name, item] : items_) {
        item->DrawInfoUI();
    }
    itemSlot_.DrawInfoUI();
}

std::shared_ptr<Item> ItemManager::GetItem(const std::string& name) {
    auto it = items_.find(name);
    if (it != items_.end()) {
        return it->second;
    }
    return nullptr;
}

bool ItemManager::AddItemToSlot(const std::string& name) {
    auto item = GetItem(name);
    if (item) {
        itemSlot_.OnTriggerItemPickup(item);
        return true;
    }
    return false;
}

std::shared_ptr<Item> ItemManager::RaycastHitItem(RaySprite& raySprite) {


    for (auto& [name, item] : items_) {
        if (!item) continue;

        const auto& obj = item->object_;
        if (!obj) continue;

        AABB box = GetAABBWorldPos(item.get()); // AABBなど
        Vector3 itemPos = item->GetWorldPosition();

        if (raySprite.IntersectsAABB(box, itemPos)) {

            obj->SetColor({ 1.0f,0.0f,0.0f,1.0f });

            if (Input::IsTriggerMouse(0)) {
                return item;
            }


        }
    }
    return nullptr;
}
