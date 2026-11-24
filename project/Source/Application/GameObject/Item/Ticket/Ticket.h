#pragma once
#include "../Item.h"
class Ticket : public Item
{
public:
    Ticket() {
        name_ = "Ticket";
        description_ = "特別展古代エジプト入場券";
        SetModel("ticket.obj");
        object_->SetObjectName(name_);
        object_->RegisterObject();
        useStage_ = "InformationStage";
    }

    ~Ticket() { object_->UnRegisterObject(); }
     void Init();
    void Use() override { 
        isGet_ = true;

    }

    void Update()override;
private:
    float generateAnimationTimer_ = 0.0f;
};

