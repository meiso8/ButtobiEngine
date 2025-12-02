#pragma once
#include"SceneManager.h"
class ResultScene :public BaseScene
{
public:
    ResultScene();
    ~ResultScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
   void SceneChangeUpdate()override;
};
