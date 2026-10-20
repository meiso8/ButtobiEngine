#include "AmenEasyStage.h"


#include"SoundManager/SoundManager.h"
#include"Sound.h"
#include"../StageManager/StageManager.h"
#include"FlagManager/FlagManager.h"
#include"TextureFactory.h"

AmenEasyStage::AmenEasyStage()
{
    slidePuzzleSystem_ = std::make_unique<SlidePuzzleSystem>(3,3,TextureFactory::Handle::PUZZLE1);
    backGround_ = std::make_unique<BackGround>();
}

AmenEasyStage::~AmenEasyStage()
{
    slidePuzzleSystem_.reset();
}

void AmenEasyStage::Initialize()
{
    //スライドパズルシステムの初期化
    slidePuzzleSystem_->Initialize({0.0f,1.25f,-1.0f},Math::kPi);
}

void AmenEasyStage::StageTransitionInitialize()
{

    //ステージのロード
    LoadAndCreateObject("AmenEasyStage_objectEditor");
    //メモの生成
    GetMemoManager()->GenerateMemos({ MemoManager::TEXT1 });
    
    backGround_->Initialize();
    // ミイラ前に移動
    GetPlayer()->Init({ 0.0f, 0.0f, -5.0f });

    GetItemManager()->GenerateItems({});
    ////アイテムの生成を初期化する
    isGenerateItem_ = false;

    //スライドパズルシステム
    slidePuzzleSystem_->SetCurrentPtr();
}

void AmenEasyStage::Update()
{

    slidePuzzleSystem_->Update(*GetUIManager()->GetCurPosPtr());

    backGround_->Update();

    if (!isGenerateItem_ && slidePuzzleSystem_->GetIsGameEnd()) {
        //太陽円盤を生成する
        GetItemManager()->GenerateItems({ "SolarDisc" });

        auto item = GetItemManager()->GetItem("SolarDisc");
        item->SetTranslate(slidePuzzleSystem_->GetPuzzleObj()->GetWorldPosition() + Vector3{0.0f,- 0.15f, 0.0f});

        FlagManager::SetFlag("ClearAmenEasyPuzzle");
        isGenerateItem_ = true;
    }

}

void AmenEasyStage::Draw()
{
    backGround_->Draw();

    slidePuzzleSystem_->Draw();

}

void AmenEasyStage::DrawSprite()
{
    slidePuzzleSystem_->DrawUI();
}


void AmenEasyStage::CheckCollision(CollisionManager& collisionManager, RaySprite& raySprite)
{
    if (!slidePuzzleSystem_->GetIsGameEnd()) {
        //アイテムを生成していないときスライドパズルのレイキャストをヒットさせる
        slidePuzzleSystem_->RayCastHit(raySprite);
    }

    collisionManager.AddCollider(slidePuzzleSystem_->GetPuzzleObj());

    // 壁との当たり判定
    for (auto& [type, object] : backGround_->GetBuilding()->GetFieldPoses()) {
        collisionManager.AddCollider(object.get());
    }

}



