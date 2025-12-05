#pragma once
#include "Vector2.h"
#include "Random.h"

class Shake {
public:
    void Start(float moveRange, uint32_t frame);
    void Update();
    bool IsActive() const { return shakeFrame_ > 0; }
    Vector2 GetOffset() const { return offset_; }
    void Reset();

private:
    Vector2 offset_ = { 0.0f, 0.0f };
    float moveRange_ = 0.0f;
    uint32_t shakeFrame_ = 0;
};
