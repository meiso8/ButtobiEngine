#include "Shake.h"

void Shake::Start(float moveRange, uint32_t frame) {
    if (shakeFrame_ > 0) {
        return; // すでにシェイク中なら無視
    }
    moveRange_ = moveRange;
    shakeFrame_ = frame;
}

void Shake::Update() {

    if (shakeFrame_ == 0) {
        offset_ = { 0.0f, 0.0f };
        return;
    }

    shakeFrame_--;

    if (shakeFrame_ % 6 == 0 && moveRange_ > 0.0f) {
        moveRange_--;
    }

    Random::SetMinMax(-moveRange_, moveRange_);
    offset_.x = Random::Get();
    offset_.y = Random::Get();
}

void Shake::Reset() {
    shakeFrame_ = 0;
    moveRange_ = 0.0f;
    offset_ = { 0.0f, 0.0f };
}
