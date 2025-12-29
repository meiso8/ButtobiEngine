#include "EnemyManager.h"

EnemyManager::EnemyManager()
{
    enemy_ = std::make_unique<Enemy>();
    enemyBulletManager_ = std::make_unique<EnemyBulletManager>();
    enemyShotBulletManager_ = std::make_unique<EnemyShotBulletManager>(enemy_.get(), enemyBulletManager_.get());

}

void EnemyManager::Initialize()
{
    enemy_->Init();
    enemyBulletManager_->Initialize();
    enemyShotBulletManager_->Initialize();
}

void EnemyManager::Update()
{
    enemy_->Update();
    enemyBulletManager_->Update();
    enemyShotBulletManager_->Update();
}

void EnemyManager::Draw(Camera& camera)
{
    enemy_->Draw(camera, kLightModeHalfL);

    enemyBulletManager_->Draw(camera, LightMode::kLightModeHalfL);
}

void EnemyManager::SetTarget(Vector3& target)
{
    enemy_->SetTarget(target);
}
