#pragma once
#include<cstdint>

class SceneChange
{
public:
    void Initialize();
    void UpdateStart(uint32_t endTime);
    /// @brief タイマーなどの更新処理
    /// @param endTime シーン遷移終了時間
    void UpdateEnd(uint32_t endTime);

    uint32_t startTimer_ = 0;
    uint32_t endTimer_ = 0;
    bool isEndScene_ = false;
    bool isSceneStart_ = false;
};

