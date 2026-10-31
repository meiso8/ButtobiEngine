#pragma once
#include"Input.h"

class InputBind
{
public:
    static bool IsClick();
    static bool IsClickPress();

    static bool IsPressMoveL();
    static bool IsPressMoveR();
    static bool IsPressMoveF();
    static bool IsPressMoveB();

    static bool IsTriggerMoveL();
    static bool IsTriggerMoveR();
    static bool IsTriggerMoveF();
    static bool IsTriggerMoveB();

    static bool IsPressSpeedButton();
    static bool IsTriggerJump();
    static bool IsClickR();
    static bool IsClickPressR();
    static bool IsTriggerMenuKey();

};

