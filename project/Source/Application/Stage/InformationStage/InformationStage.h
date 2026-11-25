#pragma once
#include"../Stage.h"
#include"InformationRoom/InformationRoom.h"
#include"Gate/Gate.h"
#include"GateSensor/GateSensor.h"
#include"UI/PasswordText/PasswordText.h"

class World;
class TicketMac;

class InformationStage :public Stage
{
private:
    //部屋
    std::unique_ptr<InformationRoom>room_ = nullptr;
    //ワールド
    std::unique_ptr<World>world_ = nullptr;
    //ゲート
    std::unique_ptr<Gate>gate_ = nullptr;
    //ゲート用センサー
    std::unique_ptr<GateSensor>gateSensor_ = nullptr;
    //チケット番号入力システム
    std::unique_ptr<PasswordText>passwordText_ = nullptr;
    //チケット販売機
    std::unique_ptr<TicketMac>ticketMac_ = nullptr;
    bool isGenerateTicket_ = false;
 
    void GenerateTicket();
public:
    InformationStage();
    void Initialize()override;
    void StageTransitionInitialize()override;
    void Update()override;
    void Draw()override;
    void CheckCollision(CollisionManager& collisionManager, RaySprite& raySprite)override;
    void DrawSprite()override;
};

