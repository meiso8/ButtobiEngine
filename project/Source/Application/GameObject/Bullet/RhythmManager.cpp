#include "RhythmManager.h"
#include"Window.h"

void RhythmManager::Initialize() {
    timer_ = 0.0f;
    beats_.clear();
    // 例：4拍子で1秒ごとにビートを追加
    for (int i = 0; i < 100; ++i) {
        beats_.push_back({ i * 1.0f, false });
    }
}

void RhythmManager::Update() {
    timer_ += InverseFPS;
}

void RhythmManager::AddBeat(float time)
{
}

bool RhythmManager::IsOnBeat(float currentTime, float tolerance) {
    for (auto& beat : beats_) {
        if (!beat.hit && std::abs(currentTime - beat.time) <= tolerance) {
            beat.hit = true;
            return true;
        }
    }
    return false;
}
