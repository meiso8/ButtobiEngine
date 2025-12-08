#include "EnemyShotWaveManager.h"

#include "Enemy.h"
#include "EnemyShockWaveManager.h"
#include"MatsumotoObj/GameSceneObj/FloorGameFloorManager.h"

#include"Sound.h"
EnemyShotWaveManager::Direction EnemyShotWaveManager::direction_ = EnemyShotWaveManager::Direction::LEFT;
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
    endPos_.x += endOffset_;
}

void EnemyShotWaveManager::Right(int& randX, int& randY)
{
    randX = 0;
    randY = rand() % kMapHeight;
    startPos_ = floorGameFloorManager_->GetFloorPos(kMapWidth - 1, randY);
    startPos_.x += offset_;
    endPos_ = floorGameFloorManager_->GetFloorPos(randX, randY);
    endPos_.x -= endOffset_;
}

void EnemyShotWaveManager::Back(int& randX, int& randY)
{
    randX = rand() % kMapWidth;
    randY = 0;
    startPos_ = floorGameFloorManager_->GetFloorPos(randX, kMapHeight - 1);
    startPos_.z += offsetBackStart_;
    endPos_ = floorGameFloorManager_->GetFloorPos(randX, randY);
    endPos_.z -= endOffset_;
}


EnemyShotWaveManager::EnemyShotWaveManager(Enemy* enemy, EnemyShockWaveManager* shockWaveManager, FloorGameFloorManager* floorGameFloorManager) :
    enemy_(enemy), shockWaveManager_(shockWaveManager), floorGameFloorManager_(floorGameFloorManager) {


    enemyPoses_[LEFT] = { -4.0f - offset_,enemyPosY_,0.0f };
    enemyPoses_[RIGHT] = { 4.0f + offset_,enemyPosY_,0.0f };
    enemyPoses_[BACK] = Vector3{ 0.0f, enemyPosY_, offset_ + 1.0f };

}

void EnemyShotWaveManager::Initialize() {
    endPos_ = { 0.0f };
    startPos_ = { 0.0f };

    for (int y = 0; y < kMapHeight; y++) {
        for (int x = 0; x < kMapWidth; x++) {
            used[x][y] = false;
        }
    }
    direction_ = static_cast<Direction>(rand() % 3);
}

void EnemyShotWaveManager::Update() {
    if (enemy_ == nullptr || shockWaveManager_ == nullptr) {
        return;
    }

    //enemy_->SetTarget(enemyPoses_[direction_]);
    // 弾投げ
    if (enemy_->phase_ == Enemy::SHOCKWAVEATTACK) {

        if (!enemy_->isWavePosSelect_) {
            //初期化する
            Initialize();
            //ターゲットを設定
            enemy_->SetTarget(enemyPoses_[direction_]);
            // 使ったらマーク
            Sound::PlayOriginSE(Sound::kWindAttackCharge);
            enemy_->isWavePosSelect_ = true;
        }

        if (enemy_->isWavePosSelect_) {
            //ショットした時
            if (enemy_->isWaveShot_) {

                int randX = 0, randY = 0;

                for (auto& wave : shockWaveManager_->GetWaves()) {

                    do {
                        //呼び出す
                        (this->*spFuncTable[static_cast<size_t>(direction_)])(randX, randY);

                    } while (IsOccupied(randX, randY)); // ← ここでかぶりチェック！


                    used[randX][randY] = true;

                    startPos_.y += 1.0f;
                    endPos_.y += 1.0f;

                    if (direction_ != BACK) {
                        shockWaveManager_->ShotWave(startPos_, endPos_, EnemyShockWave::kHorizontal);
                    } else {
                        shockWaveManager_->ShotWave(startPos_, endPos_, EnemyShockWave::kVertical);
                    }
                }

                enemy_->isWaveShot_ = false;
                enemy_->isWavePosSelect_ = false;
          
            }
        }
  

    }

}
