#pragma once
#include"SceneManager.h"

class TitleScene :public SceneManager {

public:
    void Initialize()override;
    void Update()override;
    void Draw()override;
};
