#pragma once
#include<cstdint>

class SceneChange
{
public:
    void Initialize();
    /// @brief タイマーなどの更新処理
    /// @param endTime シーン遷移終了時間
    void Update(uint32_t endTime);
    uint32_t timer_ = 0;
    bool isEndScene_ = false;

};

