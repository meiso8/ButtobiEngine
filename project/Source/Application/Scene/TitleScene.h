#pragma once
#include"SceneManager.h"
#include"Puzzle/Puzzle.h"

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

private:
    std::unique_ptr<Puzzle>puzzle_ = nullptr;
};

