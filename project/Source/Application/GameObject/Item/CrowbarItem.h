#pragma once

#include"ItemSlot.h"

class CrowbarItem : public Item {
public:

    CrowbarItem() {
        name_ = "CrowbarItem";
        description_ = "固く閉ざされた扉をこじ開けることができる。";
        SetModel(ModelManager::KEY_A);
    }

    std::shared_ptr<Item> CombineWith(const std::shared_ptr<Item>& other) override;

    void Use() override {
        //TryOpenNearbyDoor();
    }
};
