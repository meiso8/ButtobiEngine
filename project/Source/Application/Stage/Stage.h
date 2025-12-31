#pragma once
#include"Player/Player.h"

class Camera;
class Stage
{
protected:
    Player* player_ = nullptr;

public:
    Stage();
    void SetPlayer(Player* player) { player_ = player; };
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw(Camera& camera) = 0;
};

