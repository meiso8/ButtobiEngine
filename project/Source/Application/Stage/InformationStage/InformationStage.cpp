#include "InformationStage.h"

#include"InputBind.h"
#include"FlagManager/FlagManager.h"
#include"TicketMac/TicketMac.h"

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

    ticketMac_ = std::make_unique<TicketMac>();

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
    GetPlayer()->Init({ 3.25f, 0.0f, -3.25f });

    gate_->Initialize();
    gateSensor_->Initialize();
    ticketMac_->Initialize();

    GetItemManager()->GenerateItems({});
}

void InformationStage::Update()
{
    room_->Update();

    gate_->SetHitSensor(gateSensor_->GetHitPlayer());
    //セットした後毎フレーム当たり判定を戻す
    gateSensor_->Update();

    auto itemManager = GetItemManager();
    auto ticket = itemManager->GetItem("Ticket");

    if (ticket) {
  
        if (ticket->IsGet()) {
            //もし、チケットを手に入れたら
            gate_->SetCanOpen(true);
            FlagManager::SetFlag("GetTicket");
        } else {
            Vector3 pos = ticketMac_->GetWorldPosition() + Vector3{ 0.2f,0.8f,-0.25f };
            ticket->SetStartEndPos(pos, pos + Vector3{ 0.0f, 0.0f, -0.05f });
            ticket->SetRotate({ Math::kPi,-Math::kHPi,0.0f });
        }
    }


    gate_->Update();
    ticketMac_->Update();

    //パスワードシステム
    passwordText_->Update();


    if (passwordText_->GetIsUnLock() || Input::IsTriggerKey(DIK_SPACE)) {
        if (!isGenerateTicket_) {
            isGenerateTicket_ = true;
            itemManager->GenerateItems({ "Ticket" });
            FlagManager::SetFlag("GenerateTicket");
            ticketMac_->SetOutline(false);
        }
    }
}

void InformationStage::Draw()
{
    room_->Draw();
    gate_->Draw();
    ticketMac_->Draw();
}

void InformationStage::CheckCollision(CollisionManager& collisionManager, RaySprite& raySprite)
{

    if (!passwordText_->GetIsUnLock()) {
        //チケット販売機と当たっていたらがあるところで
        if (raySprite.Intersect(ticketMac_.get())) {
            if (InputBind::IsClick()) {
                passwordText_->SetIsActive(true);
            }
        }
    }


    //かべ
    for (auto& [type, object] : room_.get()->GetFieldPoses()) {
        collisionManager.AddCollider(object.get());
    }

    collisionManager.AddCollider(gate_.get());
    collisionManager.AddCollider(gateSensor_.get());
    collisionManager.AddCollider(ticketMac_.get());
}

void InformationStage::DrawSprite()
{
    passwordText_->Draw();
}
