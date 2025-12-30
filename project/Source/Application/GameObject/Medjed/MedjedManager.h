#pragma once
#include"DummyMedjed.h"
#include<memory>
#include <vector>
#include "Medjed.h"
#include"Enemy/EnemyManager.h"
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
    float enemyApperTime_ = 0.0f;
    std::unique_ptr<EnemyManager>enemyManager_;
    void UpdateEnemyApperTime();
    void UpdateMedjedIfNotFind();
    void UpdateMedjedIfFind();
    void PlaceLockersRandomly();
    bool IsOverlapping(const Vector2& pos, const std::vector<Vector2>& placedPositions);
public:
    MedjedManager();
    void SetTarget(Vector3& target) { targetPos_ = &target;};
    void RayCastHit(RaySprite& raySprite);

    void Initialize();
    void Draw(Camera& camera);
    void Update();

    std::vector < std::unique_ptr<DummyMedjed>>& GetAllMedjeds() { return dummyMedjeds_; };
    Medjed* GetMedjed();
    const bool& GetIsFindMedjed() {
        return GetMedjed()->GetIsFind();
    };

    const bool& GetIsApperMedjed() {
        return GetEnemy()->GetIsApper();
    };

    void SetIsFindMedjed(const bool& flag) { GetMedjed()->SetIsFind(flag); }
    EnemyBulletManager* GetBulletManager() {
        return enemyManager_->GetBulletManager();
    }
    Enemy* GetEnemy() {
        return enemyManager_->GetEnemy();
    }
    EnemyShotBulletManager* GetShotBulletManager() {
        return enemyManager_->GetEnemyShotBulletManager();
    }
};

