#include "MummyStage.h"

#include"SoundManager/SoundManager.h"
#include"InputBind.h"

MummyStage::MummyStage()
{
    papyrus_ = std::make_unique<Papyrus>();
    mummy_ = std::make_unique<Mummy>();
}
void MummyStage::Initialize() {

    papyrus_->Initialize();
    mummy_->Initialize();
}

void MummyStage::Update() {
    papyrus_->Update();
    mummy_->Update();
}

bool MummyStage::IsRayCastHit(RaySprite& raySprite)
{
    AABB aabb = GetAABBWorldPos(mummy_.get());

    if (raySprite.IntersectsAABB(aabb, mummy_->GetWorldPosition())) {
        if (InputBind::IsClick())              
            //Openしていなかったらmummyをあける
            if (!mummy_->GetIsOpen()) {
                SoundManager::PlayCorrectSE();
                mummy_->SetIsOpen(true);
            }

        return true;
    }

    return false;
}

void MummyStage::CheckCollision(CollisionManager& collisionManager)
{
    //ミイラ
    if (IsRayCastHit(*player_->raySprite_)) {
        //心臓を使う
        itemManager_->UseItemFromSlot(GetMummy()->GetWorldPosition());
    }

    //ミイラの台も一緒に
    collisionManager.AddCollider(GetMummy());
    collisionManager.AddCollider(GetMummy()->GetPlatform());
    collisionManager.AddCollider(papyrus_.get());
}

void MummyStage::Draw(Camera& camera) {
    papyrus_->Draw(camera);
    mummy_->Draw(camera);
}
