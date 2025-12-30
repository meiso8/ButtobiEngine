#pragma once

#include"Field.h"
#include "Building.h"
#include"World.h"
#include<memory>

class BackGround
{
private:
    std::unique_ptr<World>world_ = nullptr;
    std::unique_ptr<Field>filed_ = nullptr;
    std::unique_ptr<Building>building_ = nullptr;
public:
    BackGround();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
    void UpdateApperMedjed();
    Building* GetBuilding() {
        return building_
            .get();
    };
};

