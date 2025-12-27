#pragma once
#include"ItemSlot.h"

class SunRod : public Item {
public:
    SunRod() {
        name_ = "SunRod";
        description_ = "太陽の力を宿した神聖な杖。";
        SetModel(ModelManager::KEY_SPACE);
    }

    void Use() override {
        //ActivateSunSeal();
    }
};
