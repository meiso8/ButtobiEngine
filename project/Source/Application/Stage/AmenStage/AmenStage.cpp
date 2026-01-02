#include "AmenStage.h"

#include"SoundManager/SoundManager.h"

const bool AmenStage::IsClear()
{
    return slidePuzzleSystem_->GetIsGameEnd();
}
AmenStage::AmenStage()
{
    slidePuzzleSystem_ = std::make_unique<SlidePuzzleSystem>();
    amenRa_ = std::make_unique<AmenRa>();
    backGround_ = std::make_unique<BackGround>();
}

void AmenStage::Initialize()
{

    slidePuzzleSystem_->Initialize();
    backGround_->Initialize();
    itemManager_->Init();
    amenRa_->Initialize();
}

void AmenStage::Update()
{

    SoundManager::NotFindMedjedUpdate();
    slidePuzzleSystem_->Update();
    amenRa_->Update();
    backGround_->Update();
}

void AmenStage::Draw(Camera& camera)
{
    backGround_->Draw(camera);
    amenRa_->Draw(camera);
    slidePuzzleSystem_->Draw(camera);
}

void AmenStage::DrawUI()
{
    slidePuzzleSystem_->DrawUI();
}

void AmenStage::CheckCollision(CollisionManager& collisionManager)
{
    slidePuzzleSystem_->RayCastHit(*player_->raySprite_);

    collisionManager.AddCollider(slidePuzzleSystem_->GetPuzzleObj());
    collisionManager.AddCollider(amenRa_.get());
    
        // 壁との当たり判定
    for (auto& [type, object] : backGround_->GetBuilding()->GetFieldPoses()) {
        collisionManager.AddCollider(object.get());
    }
}



