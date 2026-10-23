#pragma once
#include"Block/Block.h"
class MeltBlockMap
{
    //temperature
    enum BlockTempareture {
        kMiddle_Low,
        kMiddle_High,
        kLow,
        kHigh,
        kMaxBlockTemperature,
    };
public:
    MeltBlockMap();
    void Initialize();
    void Update();
    void Draw();

    const bool& IsClear() { return isClear_; };
    void ResetPushMap();
    void RayCastHit(RaySprite& raySprite);
    void ClearSet();
   const std::array< std::unique_ptr<Block>, kMaxBlockTemperature>& GetMap() { return blocks_; }
private:
    std::map<int, Vector4>colorMap_;
    //温度マップ
    std::map<BlockTempareture, float>temperatureMap_;
    //ブロック群
    std::array< std::unique_ptr<Block>, kMaxBlockTemperature>blocks_;
    //順番
    std::vector<int> steppedOrder_;
    //正しい順番
    std::vector<int> correctOrder_ = { kLow, kMiddle_Low, kMiddle_High, kHigh };
    //クリアフラグ
    bool isClear_ = false;
};

