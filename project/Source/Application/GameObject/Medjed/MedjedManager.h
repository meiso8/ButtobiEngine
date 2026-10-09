#pragma once
#include"DummyMedjed.h"
#include<memory>
#include <vector>
#include "Medjed.h"
#include"Enemy.h"
class Camera;
class RaySprite;

class MedjedManager
{

private:

    std::vector < std::unique_ptr<DummyMedjed>>dummyMedjeds_;
    std::unique_ptr<Enemy>enemy_ = nullptr;

    float lockerWidth = 1.0f;
    float spacing = 0.2f;
    float minDistance = lockerWidth + spacing;
    float rangeMin = -10.0f;
    float rangeMax = 10.0f;

    float enemyApperTime_ = 0.0f;
    const float kEnemyApperMaxTime_  = 4.0f;
    const float dummyMedjedHideTime_ = 6.0f;
    const Vector3* targetPos_ = nullptr;
private:

    void UpdateEnemyApperTime();
    void UpdateMedjedIfNotFind();
    void UpdateMedjedIfFind();
    void PlaceLockersRandomly();
    bool IsOverlapping(const Vector2& pos, const std::vector<Vector2>& placedPositions);
    //プライベート関数内のみアクセス可能とする


public:

    MedjedManager();
    ~MedjedManager();
    void RayCastHit(RaySprite& raySprite);

    void Initialize();
    void Draw();
    void Update();

    Enemy* GetEnemy() {
        return enemy_.get();
    }
    Medjed* GetMedjed();

    const std::vector < std::unique_ptr<DummyMedjed>>& GetAllMedjeds() { return dummyMedjeds_; };
    const bool& GetIsFindMedjed() { return GetMedjed()->GetIsFind();};
    const WorldTransform& GetMedjedWorldTransform() { return GetMedjed()->GetWorldTransform(); }
    void SetIsFindMedjed(const bool& flag) { GetMedjed()->SetIsFind(flag); };


    const bool& GetIsApperMedjed() {  return enemy_->GetIsApper(); };
    const bool& GetIsEnemyDead() { return enemy_->GetIsDead(); }
    const WorldTransform& GetEnemyWorldTransform()const { return enemy_->GetWorldTransform(); }
    const Enemy::PHASE GetEnemyPhase();
    const HPs* GetEnemyHPsPtr() { return enemy_->GetHpsPtr(); }


    void SetTargetPos(const Vector3* pos) { targetPos_ = pos; }
};

