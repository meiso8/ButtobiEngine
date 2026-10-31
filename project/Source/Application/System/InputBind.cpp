#include "InputBind.h"

bool InputBind::IsClick() {
    return (Input::IsTriggerMouse(0) || Input::IsControllerTriggerButton(XINPUT_GAMEPAD_A, 0));
}

bool InputBind::IsClickPress() {
    return (Input::IsPressMouse(0) || Input::IsControllerPressButton(XINPUT_GAMEPAD_LEFT_SHOULDER, 0));
}

bool InputBind::IsPressMoveL() {
    return (Input::IsPressKey(DIK_A)|| Input::IsControllerPressButton(XINPUT_GAMEPAD_DPAD_LEFT, 0));
};
bool InputBind::IsPressMoveR() {
    return (Input::IsPressKey(DIK_D)|| Input::IsControllerPressButton(XINPUT_GAMEPAD_DPAD_RIGHT, 0));
};
bool InputBind::IsPressMoveF() {
    return (Input::IsPressKey(DIK_W)|| Input::IsControllerPressButton(XINPUT_GAMEPAD_DPAD_UP, 0));
};
bool InputBind::IsPressMoveB() {
    return (Input::IsPressKey(DIK_S)|| Input::IsControllerPressButton(XINPUT_GAMEPAD_DPAD_DOWN, 0));
}
bool InputBind::IsTriggerMoveL()
{
    return (Input::IsTriggerKey(DIK_A) || Input::IsControllerTriggerButton(XINPUT_GAMEPAD_DPAD_LEFT, 0));
}
bool InputBind::IsTriggerMoveR()
{
    return (Input::IsTriggerKey(DIK_D) || Input::IsControllerTriggerButton(XINPUT_GAMEPAD_DPAD_RIGHT, 0));
}
bool InputBind::IsTriggerMoveF()
{
    return (Input::IsTriggerKey(DIK_W) || Input::IsControllerTriggerButton(XINPUT_GAMEPAD_DPAD_UP, 0));
}
bool InputBind::IsTriggerMoveB()
{
    return (Input::IsTriggerKey(DIK_S) || Input::IsControllerTriggerButton(XINPUT_GAMEPAD_DPAD_DOWN, 0));
}
bool InputBind::IsPressSpeedButton()
{
    return  (Input::IsPressKey(DIK_LSHIFT));
}
bool InputBind::IsTriggerJump()
{
    return Input::IsTriggerKey(DIK_SPACE) || Input::IsControllerTriggerButton(XINPUT_GAMEPAD_X, 0);
}
bool InputBind::IsClickR()
{
    return Input::IsTriggerMouse(1) ||Input::IsControllerTriggerButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, 0);
}
bool InputBind::IsClickPressR()
{
    return Input::IsPressMouse(1) || Input::IsControllerPressButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, 0);
}
bool InputBind::IsTriggerMenuKey()
{
    //ちょっと後でキーを確認する
    return (Input::IsTriggerKey(DIK_ESCAPE) || Input::IsControllerTriggerButton(XINPUT_GAMEPAD_START, 0));
};