#include "WaterStage.h"
#include"SoundManager/SoundManager.h"
#include"Sound.h"
#include"RaySprite/RaySprite.h"

WaterStage::WaterStage()
{
    water_ = std::make_unique<Water>();
    papyrusWall_ = std::make_unique<PapyrusWall>();
    blockMap_ = std::make_unique<BlockMap>();
}

void WaterStage::Initialize()
{
    water_->Initialize();

    papyrusWall_->Init();
    papyrusWall_->Update();
    blockMap_->Initialize();
    blockMap_->Update();

}

void WaterStage::StageTransitionInitialize()
{
    //ステージのロード
    LoadAndCreateObject("WaterStage_objectEditor");

    //メモ管理
    GetMemoManager()->GenerateMemos({ TextureFactory::MEMO2, TextureFactory::BOOK2 });
    //プレイヤー管理
    GetPlayer()->Init({ 0.0f,0.0f,-10.0f });
    //アイテム管理　なにも生成しない
    GetItemManager()->GenerateItems({});

    itemApper_ = false;
}

void WaterStage::Update()
{

    blockMap_->Update();

    if (blockMap_->IsClear()) {
        //クリアしていたら水が引ける
        water_->SetIsDrain(true);
        if (!itemApper_) {
            itemApper_ = true;
            GetItemManager()->GenerateItems({ "GoldHeart" });
        }

    }

    water_->Update();
    papyrusWall_->Update();

    //オブジェクトの更新
    UpdateObject();
}

void WaterStage::Draw()
{
    papyrusWall_->Draw();
    blockMap_->Draw();
    //オブジェクトの描画
    DrawObject();
    water_->Draw();

}

void WaterStage::CheckCollision(CollisionManager& collisionManager, RaySprite& raySprite)
{

    blockMap_->RayCastHit(raySprite);

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

    //コライダーを追加する
    AddObjectCollision(collisionManager);
}

