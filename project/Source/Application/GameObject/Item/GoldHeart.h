#pragma once
#include "ItemSlot.h"

class GoldHeart : public Item {
public:
     void Init()override;
    GoldHeart() {
        name_ = "GoldHeart";
        description_ = "何かにはめ込むことで力を発揮する。";
        SetModel(ModelManager::HART);
        object_->SetLightMode(kLightModeLReflectance);
    }
    void Update()override;
    void Use() override;
};
