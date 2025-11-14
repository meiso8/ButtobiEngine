#pragma once
#include"SceneManager.h"
class ResultScene :public SceneManager
{
public:
    ResultScene();
    ~ResultScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
};
