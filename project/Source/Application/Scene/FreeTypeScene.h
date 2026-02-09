#pragma once
#include "SceneManager.h"

#include"../Engine/FreeTypeManager/FreeTypeManager.h"
#include"../Engine/FreeTypeManager/Text.h"

class FreeTypeScene : public BaseScene
{
public:
    FreeTypeScene();
    void Initialize()override;
    void Update()override;
    void Draw()override;
private:
    Text text_;
    uint32_t handle_;
    Text pressSpaceText_;
};

