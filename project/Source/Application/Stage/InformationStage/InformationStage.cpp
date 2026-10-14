#include "InformationStage.h"

#include"InputBind.h"
#include"FlagManager/FlagManager.h"

InformationStage::InformationStage()
{
    room_ = std::make_unique<InformationRoom>();
    //ゲート
    gate_ = std::make_unique<Gate>();
    //ゲート用センサー
    gateSensor_ = std::make_unique<GateSensor>();

    gateSensor_->SetParent(gate_->GetWorldTransform());
    //チケット番号システム
    passwordText_ = std::make_unique<PasswordText>();
    passwordText_->SetCurPosPtr(GetUIManager()->GetCurPosPtr());
}

void InformationStage::Initialize()
{
    room_->Init();
    room_->Update();

    passwordText_->Initialize();

    isGenerateTicket_ = false;

}

void InformationStage::StageTransitionInitialize()
{

    //ステージのロード
    LoadAndCreateObject("Information_objectEditor");

    GetMemoManager()->GenerateMemos({});
    // ミイラ前に移動
    GetPlayer()->Init({3.25f, 0.0f, -3.25f});

    gate_->Initialize();
    gateSensor_->Initialize();

    GetItemManager()->GenerateItems({});
}

void InformationStage::Update()
{
    room_->Update(); 

    gate_->SetHitSensor(gateSensor_->GetHitPlayer());
    //セットした後毎フレーム当たり判定を戻す
    gateSensor_->Update();

    auto itemManager = GetItemManager();
    auto item = itemManager->GetItem("Ticket");

    if (item&& item->IsGet()) {
        //もし、チケットを手に入れたら
        gate_->SetCanOpen(true);
        FlagManager::SetFlag("GetTicket");
    }

    gate_->Update();

    //パスワードシステム
    passwordText_->Update();

    if (passwordText_->GetIsUnLock()) {
        if (!isGenerateTicket_) {
            isGenerateTicket_ = true;
            itemManager->GenerateItems({ "Ticket" });
            FlagManager::SetFlag("GenerateTicket");
        }
    }
}

void InformationStage::Draw()
{
    room_->Draw();
    gate_->Draw();
}

void InformationStage::CheckCollision(CollisionManager& collisionManager, RaySprite& raySprite)
{


    //チケット販売機と当たっていたらがあるところで

    for (auto& obj : GetObjectSet()) {

        if (obj->collider_->GetCollisionAttribute() == CollisionTag::GetTag("TicketMac")) {
            auto* collider = obj->collider_.get();
            if (raySprite.Intersect(collider)) {
                if (InputBind::IsClick()) {
                    passwordText_->SetIsActive(true);
                    break;
                }
            }
        }
    }


    //かべ
    for (auto& [type, object] : room_.get()->GetFieldPoses()) {
        collisionManager.AddCollider(object.get());
    }

    collisionManager.AddCollider(gate_.get());
    collisionManager.AddCollider(gateSensor_.get());

}

void InformationStage::DrawSprite()
{
    passwordText_->Draw();
}
