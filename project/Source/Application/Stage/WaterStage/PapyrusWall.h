#pragma once
#include"BackGround/Building.h"

class PapyrusWall:public Building
{
public:
    PapyrusWall();
    void Draw(Camera& camera)override;
private:
    void SetWallAABB()override;
    void SetWallPos()override;

};

