#include "EnemyFloorChangeManager.h"

#include "Enemy.h"
#include "EnemyBombManager.h"
#include"MatsumotoObj/GameSceneObj/FloorGameFloorManager.h"

#include"Sound.h"
EnemyFloorChangeManager::EnemyFloorChangeManager(Enemy* enemy, EnemyBombManager* bombManager, FloorGameFloorManager* floorGameFloorManager) :
    enemy_(enemy), bombManager_(bombManager), floorGameFloorManager_(floorGameFloorManager) {
}

void EnemyFloorChangeManager::Initialize() {
    for (int y = 0; y < kMapHeight; y++) {
        for (int x = 0; x < kMapWidth; x++) {
            used[x][y] = false;
        }
    }
    isSelected_ = false;
}

void EnemyFloorChangeManager::Update() {
    if (enemy_ == nullptr || bombManager_ == nullptr) {
        return;
    }

    // 弾投げ
    if (enemy_->isBombShot_) {
       
        int randX, randY;
        static Vector3 endPos;

        for (auto& bomb : bombManager_->GetBombs()) {

            SelectFloor(randX, randY,endPos);

            Vector3 shotPosition = enemy_->GetWorldPosition() + Vector3{ 0.0f,0.0f,-1.0f } *0.5f;
            bombManager_->ShotBomb(shotPosition, endPos, shotSize_);

        }
        Initialize();

        enemy_->isBombShot_ = false;
    }

    if (enemy_->isGoToRanDomFloor_) {
        //ランダム位置に移動する
        if (!isSelected_) {
            isSelected_ = true;
            int randX, randY;
            static Vector3 targetPos;
            //ランダム位置を記録
            SelectFloor(randX, randY, targetPos);
            //敵のターゲットを設定
            targetPos.y += 1.0f;
            enemy_->SetTarget(targetPos);
        }

        //敵が床を取ったらフラグを初期化する
        if (enemy_->isFloorBring_) {
            enemy_->isGoToRanDomFloor_ = false;
            Initialize();
        }
       
    }
}

void EnemyFloorChangeManager::SelectFloor(int& randX,int&randY, Vector3& endPos)
{
    do {
        randX = rand() % kMapWidth;
        randY = rand() % kMapHeight;
        endPos = floorGameFloorManager_->GetFloorPos(randX, randY);
    } while (IsOccupied(randX, randY)); // ← ここでかぶりチェック！

    // 使ったらマーク
    used[randX][randY] = true;
}


