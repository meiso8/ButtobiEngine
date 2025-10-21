#pragma once
#define DIRECTINPUT_VERSION 0x0800//DirectXバージョン指定
//　DIRECTINPUT_VERSION　dinput.hのインクルードより上に書くこと。
#include <dinput.h>
#include<cstdint>
#include"Vector2.h"
#include"Window.h"

#include"Window.h"
#include<memory>
#define InverseFPS 1.0f/60.0f

class Camera;

class Input {
public:
    static bool foundJoystick_;
    GUID joystickGUID = GUID_NULL;
    static bool isDragging_;
public:

    enum ButtonType {
        BUTTON_LEFT,
        BUTTON_RIGHT,
    };

    static Input* GetInstance();

    Input() = default;
    Input(Input&) = delete;
    Input& operator=(Input&) = delete;

    HRESULT Initialize(Window& window);
    /// @brief キーを押した状態 
    static bool IsPushKey(const uint8_t& key);
    /// @briefキーを押した瞬間
    static bool IsTriggerKey(const uint8_t& key);
    /// @briefキーを離した状態
    static bool IsReleaseStateKey(const uint8_t& key);
    /// @briefキーを離した瞬間
    static bool IsReleaseKey(const uint8_t& key);
    static bool IsAnyKeyPressed();
    /// @brief キーの情報を取得する
    void Update();
    /// @brief マウスが押されている状態かどうかを取得する
    /// @param index マウスボタンの番号　0 =左 1 = 右　2 = 中　3 = XButton2
    /// @return 押されているかどうか
    static bool IsPressMouse(uint32_t index);
    /// @brief マウスがトリガーされた状態かどうかを取得する
    /// @param index マウスボタンの番号　0 =左 1 = 右　2 = 中　3 = XButton2
    /// @return トリガーされたかどうか
    static bool IsTriggerMouse(uint32_t index);
    static bool IsJoyStickPressButton(uint32_t index);
    static bool IsJoyStickTrigger(uint32_t index);

    static bool GetJoyStickPos(float* x, float* y, ButtonType buttonType);
    static bool GetJoyStickDPadButton(float* x, float* y);

    DIJOYSTATE& GetJoyState() { return joyState_; };

    static Vector2& GetMousePos();
    static float GetMouseWheel();

private:
    static Input* instance_;
    Window* window_ = nullptr;

    IDirectInputDevice8* keyboard_ = nullptr;
    //全キー入力状態を取得する
    static BYTE key_[256];
    static BYTE preKey_[256];
    //マウス
    IDirectInputDevice8* mouse_ = nullptr;
    static DIMOUSESTATE mouseState_;
    static DIMOUSESTATE preMouseState_;	// マウス情報(変化検知用)



    //ゲームパッド
    IDirectInputDevice8* gamePad_ = nullptr;
    static DIJOYSTATE joyState_;
    static float deadZone_;
    static BYTE preJoyButtons_[32];

private:
    static bool NormalizeButtonCount(float* x, float* y, LONG& buttonLX, LONG& buttonLY);
    ~Input();
};
