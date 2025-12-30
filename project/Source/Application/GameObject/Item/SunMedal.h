#pragma once
#include"ItemSlot.h"

class SunMedal : public Item {
public:
    SunMedal() {
        name_ = "SunMedal";
        description_ = "神聖な力を秘めたメダル。";
        SetModel(ModelManager::SUN_MEDAL);
    }
    void Use() override {
        //TryOpenNearbyDoor();
    }
};
