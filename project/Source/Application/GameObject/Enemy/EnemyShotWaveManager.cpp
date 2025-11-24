#include "EnemyShotWaveManager.h"

#include "Enemy.h"
#include "EnemyShockWaveManager.h"
#include"MatsumotoObj/GameSceneObj/FloorGameFloorManager.h"

#include"Sound.h"

//テーブルにポインタを入れるために別途定義が必要　static
void(EnemyShotWaveManager::* EnemyShotWaveManager::spFuncTable[])(int& randX, int& randY) {
    &EnemyShotWaveManager::Left,
        & EnemyShotWaveManager::Right,
        & EnemyShotWaveManager::Back,
};


void EnemyShotWaveManager::Left(int& randX, int& randY)
{
    randX = kMapWidth - 1;
    randY = rand() % kMapHeight;
    startPos_ = floorGameFloorManager_->GetFloorPos(0, randY);
    startPos_.x -= offset_;
    endPos_ = floorGameFloorManager_->GetFloorPos(randX, randY);
    endPos_.x += offset_;
}

void EnemyShotWaveManager::Right(int& randX, int& randY)
{
    randX = 0;
    randY = rand() % kMapHeight;
    startPos_ = floorGameFloorManager_->GetFloorPos(kMapWidth - 1, randY);
    startPos_.x += offset_;
    endPos_ = floorGameFloorManager_->GetFloorPos(randX, randY);
    endPos_.x -= offset_;
}

void EnemyShotWaveManager::Back(int& randX, int& randY)
{
    randX = rand() % kMapWidth;
    randY = 0;
    startPos_ = floorGameFloorManager_->GetFloorPos(randX, kMapHeight - 1);
    startPos_.z += offset_;
    endPos_ = floorGameFloorManager_->GetFloorPos(randX, randY);
    endPos_.z -= offset_;
}


EnemyShotWaveManager::EnemyShotWaveManager(Enemy* enemy, EnemyShockWaveManager* shockWaveManager, FloorGameFloorManager* floorGameFloorManager) :
    enemy_(enemy), shockWaveManager_(shockWaveManager), floorGameFloorManager_(floorGameFloorManager) {
    enemyPoses_[LEFT] = { floorGameFloorManager_->GetFloorPos(0, kMapHeight / 2) + Vector3{-offset_,0.0f,0.0f} };
    enemyPoses_[RIGHT] = { floorGameFloorManager_->GetFloorPos(kMapWidth - 1,  kMapHeight / 2)+ Vector3{offset_,0.0f,0.0f} };
    enemyPoses_[BACK] = { floorGameFloorManager_->GetFloorPos(kMapWidth / 2,kMapHeight - 1) + Vector3{0.0f,0.0f,offset_} };
}

void EnemyShotWaveManager::Initialize() {
    endPos_ = { 0.0f };
    startPos_ = { 0.0f };

    for (int y = 0; y < kMapHeight; y++) {
        for (int x = 0; x < kMapWidth; x++) {
            used[x][y] = false;
        }
    }

}

void EnemyShotWaveManager::Update() {
    if (enemy_ == nullptr || shockWaveManager_ == nullptr) {
        return;
    }

    if (shockWaveManager_->isWaveReset_) {

        if (!enemy_->isBombShot_) {
            Initialize();
            shockWaveManager_->isWaveReset_ = false;
            direction_ = static_cast<Direction>(rand() % 3);
        }

    } else {

        enemy_->SetTarget(enemyPoses_[direction_]);
        // 弾投げ
        if (enemy_->isWaveShot_) {

            enemy_->isWaveShot_ = false;

            for (auto& wave : shockWaveManager_->GetWaves()) {
            
                int randX = 0, randY = 0;

                do {
                    //呼び出す
                    (this->*spFuncTable[static_cast<size_t>(direction_)])(randX, randY);

                } while (IsOccupied(randX, randY)); // ← ここでかぶりチェック！
                // 使ったらマーク

                used[randX][randY] = true;

                if (direction_ != BACK) {
                    shockWaveManager_->ShotWave(startPos_, endPos_, EnemyShockWave::kHorizontal);
                } else {
                    shockWaveManager_->ShotWave(startPos_, endPos_, EnemyShockWave::kVertical);
                }    
            
            }

        }

    }
}
