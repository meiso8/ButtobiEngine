#include "AmenStage.h"

#include"SoundManager/SoundManager.h"
#include"Sound.h"
#include"../StageManager/StageManager.h"
#include"FlagManager/FlagManager.h"

AmenStage::AmenStage()
{
    slidePuzzleSystem_ = std::make_unique<SlidePuzzleSystem>();
    amenRa_ = std::make_unique<AmenRa>();
    backGround_ = std::make_unique<BackGround>();
}

AmenStage::~AmenStage()
{
    amenRa_.reset();
    slidePuzzleSystem_.reset();
}

void AmenStage::Initialize()
{
    //スライドパズルシステムの初期化
    slidePuzzleSystem_->Initialize();

}

void AmenStage::StageTransitionInitialize()
{

    //ステージのロード
    LoadAndCreateObject("AmenStage_objectEditor");
    //像の初期化
    amenRa_->Initialize();
    //メモの生成
   GetMemoManager()->GenerateMemos({TextureFactory::MEMO1, TextureFactory::MEMO3,TextureFactory::MEMO4,TextureFactory::BOOK4});
    backGround_->Initialize();
    // ミイラ前に移動
    GetPlayer()->Init({0.0f, 0.0f, -5.0f});

    GetItemManager()->GenerateItems({});
    ////アイテムの生成を初期化する
    isGenerateItem_ = false;
}

void AmenStage::Update()
{

    slidePuzzleSystem_->Update(*GetUIManager()->GetCurPosPtr());
    amenRa_->Update();
    backGround_->Update();

    if (!isGenerateItem_ && slidePuzzleSystem_->GetIsGameEnd()) {
        //太陽円盤を生成する
        GetItemManager()->GenerateItems({"SolarDisc"});
        FlagManager::SetFlag("ClearAmenPuzzle");
        isGenerateItem_ = true;
    }

}

void AmenStage::Draw()
{
    backGround_->Draw();
    amenRa_->Draw();
    slidePuzzleSystem_->Draw();

}

void AmenStage::DrawSprite()
{
    slidePuzzleSystem_->DrawUI();
}


void AmenStage::CheckCollision(CollisionManager& collisionManager,RaySprite& raySprite)
{
    if (!slidePuzzleSystem_->GetIsGameEnd()) {
        //アイテムを生成していないときスライドパズルのレイキャストをヒットさせる
        slidePuzzleSystem_->RayCastHit(raySprite);
    }

    collisionManager.AddCollider(slidePuzzleSystem_->GetPuzzleObj());
    collisionManager.AddCollider(amenRa_.get());

    // 壁との当たり判定
    for (auto& [type, object] : backGround_->GetBuilding()->GetFieldPoses()) {
        collisionManager.AddCollider(object.get());
    }

}



