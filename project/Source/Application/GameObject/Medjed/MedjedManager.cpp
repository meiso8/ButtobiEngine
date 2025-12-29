#include "MedjedManager.h"
#include"Camera.h"
#include"CollisionConfig.h"
#include <algorithm>
#include"Random.h"
#include"Player/RaySprite.h"
#include <cmath>
#include "Vector2.h" // 必要なら自作の2Dベクトルクラスを用意してね
#include"CollisionManager.h"
#include"Input.h"
#include"SoundManager/SoundManager.h"
#include"Sound.h"
MedjedManager::MedjedManager()
{
    enemyManager_ = std::make_unique<EnemyManager>();

    dummyMedjeds_.clear();

    dummyMedjeds_.resize(46);

    for (int i = 0; i < dummyMedjeds_.size(); ++i) {
        if (i == 0) {
            // 最初の1体だけ本物のメジェド様を生成！
            dummyMedjeds_[i] = std::make_unique<Medjed>();
        } else {
            dummyMedjeds_[i] = std::make_unique<DummyMedjed>();
        }
    }
}

void MedjedManager::RayCastHit(RaySprite& raySprite) {

    for (auto& medjed : dummyMedjeds_) {

        AABB aabb = GetAABBWorldPos(medjed.get());

        if (raySprite.IntersectsAABB(aabb, medjed->GetWorldPosition())) {

            //メジェドざまを当ててないとき
            if (!GetIsFindMedjed()) {
                //Mouseをクリックしたら
                if (Input::IsTriggerMouse(0)) {

                    if (auto correctMedjed = dynamic_cast<Medjed*>(medjed.get())) {

                        SetIsFindMedjed(true);
                        correctMedjed->MoveStart();
                        SoundManager::PlayCorrectSE();
                        return;

                    } else {

                        SoundManager::PlayCancelSE();
                        Sound::PlaySE(Sound::SOTTIZYANAIWA);
                        PlaceLockersRandomly();
                        return;
                    }


                }


            }

        }


    }


}

void MedjedManager::Initialize()
{
    enemyApperTime_ = false;
    PlaceLockersRandomly();
    enemyManager_->Initialize();
    enemyManager_->SetTarget(*targetPos_);
}

void MedjedManager::Draw(Camera& camera)
{
    for (auto& locker : dummyMedjeds_) {
        locker->Draw(camera);
    }
    enemyManager_->Draw(camera);
}

void MedjedManager::UpdateApperTime()
{
    if (enemyApperTime_ == 5.0f) {
        return;
    }

    enemyApperTime_ += InverseFPS;
    enemyApperTime_ = std::clamp(enemyApperTime_, 0.0f, 5.0f);

    if (enemyApperTime_ >= 5.0f) {
        GetEnemy()->SetIsApper(true);
    }
}

void MedjedManager::Update()
{

    if (GetIsFindMedjed()) {

        UpdateApperTime();

        for (auto& locker : dummyMedjeds_) {
            locker->LookTarget(*targetPos_);
        }

        enemyManager_->Update();

    } else {
        //メジェド一つだけ
        GetMedjed()->LookTarget(*targetPos_);

        if (GetMedjed()->GetIsHit()) {
            //ランダム
            PlaceLockersRandomly();
        }
    }


    for (auto& locker : dummyMedjeds_) {

        locker->Update();
    }


}



void MedjedManager::PlaceLockersRandomly() {
    std::vector<Vector2> placedPositions;



    for (auto& locker : dummyMedjeds_) {
        locker->Init();

        Vector2 pos;

        while (true) {
            Random::SetMinMax(rangeMin, rangeMax);
            pos.x = Random::Get(); pos.y = Random::Get(); // Z座標として使う 
            if (!IsOverlapping(pos, placedPositions)) {
                placedPositions.push_back(pos);
                locker->GetWorldTransform().translate_ = { pos.x, 0.0f, pos.y };
                break; // 配置成功！ 
            }
        }
    }
}


bool MedjedManager::IsOverlapping(const Vector2& pos, const std::vector<Vector2>& placedPositions) {
    for (const auto& p : placedPositions) {
        if (Distance(p, pos) < minDistance * minDistance) {
            return true;
        }
    }
    return false;
}


Medjed* MedjedManager::GetMedjed() {

    for (auto& locker : dummyMedjeds_) {

        if (auto medjed = dynamic_cast<Medjed*>(locker.get())) {
            return medjed;
        }
    }
    return nullptr; // 見つからなかった場合
}