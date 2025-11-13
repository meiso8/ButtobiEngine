#pragma once
#include"SceneManager.h"
class TitleScene :public SceneManager
{
public:
    TitleScene();
    ~TitleScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
};

