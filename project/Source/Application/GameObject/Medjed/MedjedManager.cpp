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
MedjedManager::MedjedManager()
{
    dummyMedjeds_.clear();

    dummyMedjeds_.resize(46);

    for (int i = 0; i < dummyMedjeds_.size(); ++i) {
        if (i == 2) {
            // 最初の1体だけ本物のメジェド様を生成！
            dummyMedjeds_[i] = std::make_unique<Medjed>();
        } else {
            dummyMedjeds_[i] = std::make_unique<DummyMedjed>();
        }
    }
}

void MedjedManager::RayCastHit(RaySprite& raySprite)
{
    Medjed* medjed = GetMedjed();
    AABB aabb = GetAABBWorldPos(medjed);
    if (raySprite.IntersectsAABB(aabb)) {

        //Mouseをクリックしたら
        if (Input::IsTriggerMouse(0)) {
            if (!isBigMedjedApper_) {
                isBigMedjedApper_ = true;
                medjed->MoveStart();
                SoundManager::PlayCorrectSE();
            }
        }
    }
}

void MedjedManager::Initialize()
{
    isBigMedjedApper_ = false;
    PlaceLockersRandomly();
}

void MedjedManager::Draw(Camera& camera)
{
    for (auto& locker : dummyMedjeds_) {
        locker->Draw(camera);
    }
}

void MedjedManager::Update()
{

    if (isBigMedjedApper_) {
        for (auto& locker : dummyMedjeds_) {
            locker->LookTarget(*targetPos_);
        }

    } else {
        //メジェド一つだけ
        GetMedjed()->LookTarget(*targetPos_);
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