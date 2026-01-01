#include "WaterStage.h"
#include"SoundManager/SoundManager.h"

const bool WaterStage::IsClear()
{
    return (blockMap_->IsClear() && itemManager_->HasItem("GoldHeart"));
}
WaterStage::WaterStage()
{
    water_ = std::make_unique<Water>();
    papyrusWall_ = std::make_unique<PapyrusWall>();
    blockMap_ = std::make_unique<BlockMap>();
    memoManager_ = std::make_unique<MemoManager>();
}

void WaterStage::Initialize()
{
    water_->Initialize();
    papyrusWall_->Init();
    blockMap_->Initialize();


    memoManager_->GenerateMemos({ Texture::MEMO2, Texture::MEMO4,Texture::BOOK2 });
    //メモマネージャー
    memoManager_->Initialize();
    itemManager_->Init();
    itemApper_ = false;
}

void WaterStage::Update()
{
    SoundManager::NotFindMedjedUpdate();

    water_->Update();
    papyrusWall_->Update();
    blockMap_->Update();
    memoManager_->Update();

    if (blockMap_->IsClear()) {
        //クリアしていたら水が引ける
        water_->SetIsDrain(true);
        if (!itemApper_) {
            itemApper_ = true;
            itemManager_->GenerateItems({ "GoldHeart" });
        }
        
    }
}

void WaterStage::Draw(Camera& camera)
{
    papyrusWall_->Draw(camera);
    blockMap_->Draw(camera);
    water_->Draw(camera);
    memoManager_->Draw(camera);
}

void WaterStage::CheckCollision(CollisionManager& collisionManager)
{

    //メモがヒットしているかどうか
    memoManager_->RayCastHit(*player_->raySprite_);

    //Waterのかべ
    for (auto& [type, object] : papyrusWall_.get()->GetFieldPoses()) {
        collisionManager.AddCollider(object.get());
    }

    //マップ
    for (auto& y : blockMap_->GetMap()) {
        for (auto& x : y) {
            collisionManager.AddCollider(x.get());
        }
    }


    collisionManager.AddCollider(water_.get());
}

