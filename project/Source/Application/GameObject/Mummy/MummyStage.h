#pragma once
#include"Mummy.h"
#include<memory>

class MummyStage
{
private:
    std::unique_ptr<Mummy>mummy_ = nullptr;

public:

    void Initialize();
    void Update();
    void Draw(Camera& camera);
};

