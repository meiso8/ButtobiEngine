#include "LockerManager.h"
#include"Camera.h"
#include"CollisionConfig.h"
LockerManager::LockerManager()
{

    for (int i = 0; i < lockers1_.size(); ++i) {
        lockers1_[i] = std::make_unique<Locker>();
    }
    for (int i = 0; i < lockers2_.size(); ++i) {
        lockers2_[i] = std::make_unique<Locker>();
    }

}

void LockerManager::Initialize()
{
    for (int i = 0; i < lockers1_.size(); ++i) {
        lockers1_[i]->Init();
        lockers1_[i]->SetPosX(i * 1.0f + 1.0f);
    }
    for (int i = 0; i < lockers2_.size(); ++i) {
        lockers2_[i]->Init();
        lockers2_[i]->SetPosX(i * -1.0f - 1.0f);
    }
}

void LockerManager::Draw(Camera& camera)
{
    for (int i = 0; i < lockers1_.size(); ++i) {
        lockers1_[i]->Draw(camera);
    }
    for (int i = 0; i < lockers2_.size(); ++i) {
        lockers2_[i]->Draw(camera);
    }
}

void LockerManager::Update()
{

    for (int i = 0; i < lockers1_.size(); ++i) {
        lockers1_[i]->Update();
    }
    for (int i = 0; i < lockers2_.size(); ++i) {
        lockers2_[i]->Update();
    }

}
