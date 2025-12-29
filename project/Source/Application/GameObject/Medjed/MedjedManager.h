#pragma once
#include"DummyMedjed.h"
#include<memory>
#include <vector>
#include "Medjed.h"

class Camera;
class RaySprite;

class MedjedManager
{

private:
    std::vector < std::unique_ptr<DummyMedjed>>dummyMedjeds_;
    float lockerWidth = 1.0f;
    float spacing = 0.2f;
    float minDistance = lockerWidth + spacing;
    float rangeMin = -10.0f;
    float rangeMax = 10.0f;
    Vector3* targetPos_ = nullptr;
    bool isBigMedjedApper_ = false;
public:
    MedjedManager();
    void SetTarget(Vector3& target) { targetPos_ = &target; };
    void RayCastHit(RaySprite& raySprite);

    void Initialize();
    void PlaceLockersRandomly();
    void Draw(Camera& camera);
    void Update();
    bool IsOverlapping(const Vector2& pos, const std::vector<Vector2>& placedPositions);

    std::vector < std::unique_ptr<DummyMedjed>>& GetAllMedjeds() { return dummyMedjeds_; };
    Medjed* GetMedjed();
};

