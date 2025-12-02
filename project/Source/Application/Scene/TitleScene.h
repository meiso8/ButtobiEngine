#pragma once
#include"SceneManager.h"
class TitleScene :public BaseScene
{
public:
    TitleScene();
    ~TitleScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
   void SceneChangeUpdate()override;
};

