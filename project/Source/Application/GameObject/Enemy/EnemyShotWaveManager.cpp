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

void EnemyShotWaveManager::Shot()
{

    if (!enemy_->isWaveShot_) {
        return;
    }

    int randX = 0, randY = 0;

    for (size_t j = 0; j < 4 ; ++j) {
  
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

    isSelectDirection_ = false;
    enemy_->isWaveShot_ = false;
}

void EnemyShotWaveManager::SelectDirection()
{

    if (isSelectDirection_) {
        return;
    }

    isSelectDirection_ = true;

    direction_ = static_cast<Direction>(rand() % 3);
    //初期化する
    Initialize();
    //ターゲットを設定
    enemy_->SetTarget(enemyPoses_[direction_]);
   Sound::PlaySE(Sound::kWindAttackCharge);
}


EnemyShotWaveManager::EnemyShotWaveManager(Enemy* enemy, EnemyShockWaveManager* shockWaveManager, FloorGameFloorManager* floorGameFloorManager) :
    enemy_(enemy), shockWaveManager_(shockWaveManager), floorGameFloorManager_(floorGameFloorManager) {


    enemyPoses_[LEFT] = { -4.0f - offset_,enemyPosY_,0.0f };
    enemyPoses_[RIGHT] = { 4.0f + offset_,enemyPosY_,0.0f };
    enemyPoses_[BACK] = Vector3{ 0.0f, enemyPosY_, offset_ + 4.0f };

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

    // 弾投げ
    if (enemy_->phase_ == Enemy::SHOCKWAVEATTACK) {
        //場所を選択するかどうか
        if (enemy_->isWavePosSelectStart_) {
            SelectDirection();
       
        };

        //方向を設定したとき
        if ( isSelectDirection_) {
            Shot();
        }
    }


}
