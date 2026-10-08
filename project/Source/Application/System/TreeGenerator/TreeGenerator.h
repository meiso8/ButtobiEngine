#pragma once
#include<Vector2.h>
#include<vector>
#include<memory>
#include"AABB.h"

class Object3d;
class TreeGenerator
{

public:

    TreeGenerator();
    ~TreeGenerator();
    void Initialize();
    void Update();
    void Draw();
    void Debug();


    // 除外領域を外部から設定したい場合
    void AddExcludeArea(const AABB& aaabb) { excludeAreas_.push_back(aaabb); }
    void ClearExcludeAreas() { excludeAreas_.clear(); }

private:
    int generateNum_ = 100;
    float rangeMin_ = -75.0f;
    float rangeMax_ =75.0f;
    float minDistance_ = 1.0f;
    std::vector<std::unique_ptr<Object3d>>trees_{};
    // 除外エリアのリスト
    std::vector<AABB> excludeAreas_{};
private:
    void PlaceLockersRandomly();
    bool IsOverlapping(const Vector2& pos, const std::vector<Vector2>& placedPositions);
    // 生成位置が建物の領域内にあるか確認する関数
    bool IsInExcludeArea(const Vector2& pos) const;
    void Save();
    void SaveAABB();
    void LoadAABB();
};

