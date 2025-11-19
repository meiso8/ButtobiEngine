#pragma once
#define DIRECTINPUT_VERSION 0x0800//DirectXバージョン指定
//　DIRECTINPUT_VERSION　dinput.hのインクルードより上に書くこと。
#include <dinput.h>
#include<cstdint>
#include"Vector2.h"
#include"Window.h"

#include<memory>
#define InverseFPS 1.0f/60.0f

#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

class Camera;

enum ButtonType {
    BUTTON_LEFT,
    BUTTON_RIGHT,
};

class Input {
public:
    static bool isDragging_;
public:

    Input() = default;
    Input(Input&) = delete;
    Input& operator=(Input&) = delete;
    ~Input();

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
    /// @brief マウスの座標を得る
    /// @return マウスの座標
    static Vector2& GetMousePos();
    /// @brief フィルタリングされた座標を得る
    /// @return フィルタリングされた座標
    static Vector2& GetMousePosFiltered();
    /// @brief マウスのホイール値を得る
    /// @return マウスのホイール値
    static float GetMouseWheel();
    /// @brief コントローラーが接続されているかどうか
    /// @param dwUserIndex コントローラーのインデックス
    /// @return コントローラー接続判定
    static bool IsControllerConnected(DWORD dwUserIndex);
    /// @brief コントローラーの正規化されたスティックの座標(-1~1)を得る
    /// @param type BUTTON_LEFTかBUTTON_RIGHTをいれる
    /// @param dwUserIndex コントローラーのインデックス
    /// @return 正規化されたスティックの座標(-1~1)
    static Vector2 GetControllerStickPos(ButtonType type, DWORD dwUserIndex);
    /// @brief コントローラーを振動させる
    /// @param dwUserIndex コントローラーのインデックス
    /// @param leftMotor 左モーターの振動値
    /// @param rightMotor  右モーターの振動値
    static void VibrateController(DWORD dwUserIndex, WORD leftMotor, WORD rightMotor);
    /// @brief コントローラーのボタンを押している判定
    /// @param button ボタンの値
    /// @param dwUserIndex コントローラーのインデックス
    /// @return 判定結果
    static bool IsControllerPressButton(UINT16 button, DWORD dwUserIndex);
    /// @brief コントローラーのボタントリガー判定
    /// @param button ボタンの値
    /// @param dwUserIndex コントローラーのインデックス
    /// @return 判定結果
    static bool IsControllerTriggerButton(UINT16 button, DWORD dwUserIndex);
    /// @brief コントローラーのLTRTのボタンが押されているかどうかを得る
    /// @param type BUTTON_LEFTかBUTTON_RIGHTをいれる
    /// @param dwUserIndex コントローラーのインデックス
    /// @return 判定結果
    static bool IsControllerPressLTRT(ButtonType type, DWORD dwUserIndex);
    /// @brief コントローラーのLTRTのボタンがトリガーされたかどうかを得る
    /// @param type BUTTON_LEFTかBUTTON_RIGHTをいれる
    /// @param dwUserIndex コントローラーのインデックス
    /// @return 判定結果
    static bool IsControllerTriggerLTRT(ButtonType index, DWORD dwUserIndex);
    /// @brief コントローラーのLTRTのボタンの値を得る
    /// @param type BUTTON_LEFTかBUTTON_RIGHTをいれる
    /// @param dwUserIndex コントローラーのインデックス
    /// @return LTRTのボタンの値
    static BYTE GetControllerTriggerCount(ButtonType type, DWORD dwUserIndex);

    static bool IsControllerDeadZone(BYTE& triggerButton);
private:
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
    static float deadZone_;
    static WORD preWButtons_;
    static BYTE  preBLeftTrigger_;
    static BYTE  preBRightTrigger_;
    static XINPUT_STATE xinputState_;

private:
    static Vector2 NormalizeButtonCount(SHORT& buttonLX, SHORT& buttonLY);
    static XINPUT_STATE& GetControllerState(DWORD& dwUserIndex);
};
