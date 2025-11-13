#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

enum FloorGameKeyBind
{
	// キーボード
	K_MoveLeft = DIK_A,
	K_MoveRight = DIK_D,
	K_MoveForward = DIK_W,
	K_MoveBackward = DIK_S,
	K_Stript = DIK_SPACE,
	K_Shot = DIK_SPACE,
	// コントローラー
	C_MoveLeft = 14,
	C_MoveRight = 15,
	C_MoveForward = 12,
	C_MoveBackward = 13,
	C_Stript = 0,
	C_Shot = 0,
};