#include "EnemyShotBombManager.h"

#include "Enemy.h"
#include "EnemyBombManager.h"
#include"MatsumotoObj/GameSceneObj/FloorGameFloorManager.h"

#include"Sound.h"
EnemyShotBombManager::EnemyShotBombManager(Enemy* enemy, EnemyBombManager* bombManager, FloorGameFloorManager* floorGameFloorManager) :
    enemy_(enemy), bombManager_(bombManager), floorGameFloorManager_(floorGameFloorManager) {
}

void EnemyShotBombManager::Initialize() {

}

void EnemyShotBombManager::Update() {
    if (enemy_ == nullptr || bombManager_ == nullptr) {
        return;
    }

    if (bombManager_->isBombReset_) {

        if (!enemy_->isBombShot_) {

            for (int y = 0; y < kMapHeight; y++) {
                for (int x = 0; x < kMapWidth; x++) {
                    used[x][y] = false;
                }
            }

            bombManager_->isBombReset_ = false;

        }
     
    } else {
        // 弾投げ
        if (enemy_->isBombShot_) {
            Sound::PlaySE(Sound::PICO);
            enemy_->isBombShot_ = false;

            int randX, randY;
            static Vector3 endPos;

            do {
                randX = rand() % kMapWidth;
                randY = rand() % kMapHeight;
                endPos = floorGameFloorManager_->GetFloorPos(randX, randY);
            } while (IsOccupied(randX, randY)); // ← ここでかぶりチェック！

            // 使ったらマーク
            used[randX][randY] = true;
            enemy_->SetTarget(endPos);
            Vector3 shotPosition = enemy_->GetWorldPosition() + Vector3{ 0.0f,0.0f,-1.0f } *0.5f;
            bombManager_->ShotBomb(shotPosition, endPos, shotSize_);
        }

    }
}
