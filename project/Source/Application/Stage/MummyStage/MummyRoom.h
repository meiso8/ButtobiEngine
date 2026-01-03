#pragma once
#include "BackGround/Building.h"

class MummyRoom : public Building
{
public:
    MummyRoom();
    void Draw(Camera& camera) override;

private:
    void SetWallPos() override;
    void SetWallAABB() override;
};
