#pragma once
#include "ItemSlot.h"

class GoldHeart : public Item {
public:
    GoldHeart() {
        name_ = "GoldHeart";
        description_ = "何かにはめ込むことで力を発揮する。";
        SetModel(ModelManager::HART);
        object_->SetLightMode(kLightModeLReflectance);
    }
    void Use() override {
        //TryOpenNearbyDoor();
    }
};
