#include "KeyBindConfig.h"
#include "Input.h"

KeyBindConfig::KeyBindConfig() {
	keyBinds_["MoveLeft"].keyboardKeys.push_back(DIK_A);
	keyBinds_["MoveRight"].keyboardKeys.push_back(DIK_D);
	keyBinds_["MoveForward"].keyboardKeys.push_back(DIK_W);
	keyBinds_["MoveBack"].keyboardKeys.push_back(DIK_S);
	keyBinds_["MoveLeft"].keyboardKeys.push_back(DIK_LEFT);
	keyBinds_["MoveRight"].keyboardKeys.push_back(DIK_RIGHT);
	keyBinds_["MoveForward"].keyboardKeys.push_back(DIK_UP);
	keyBinds_["MoveBack"].keyboardKeys.push_back(DIK_DOWN);
	keyBinds_["Stript"].keyboardKeys.push_back(DIK_SPACE);
	keyBinds_["Shot"].keyboardKeys.push_back(DIK_SPACE);
	keyBinds_["Menu"].keyboardKeys.push_back(DIK_ESCAPE);
	keyBinds_["Menu"].keyboardKeys.push_back(DIK_E);

	keyBinds_["MoveLeft"].controllerButtons.push_back(XINPUT_GAMEPAD_DPAD_LEFT);
	keyBinds_["MoveRight"].controllerButtons.push_back(XINPUT_GAMEPAD_DPAD_RIGHT);
	keyBinds_["MoveForward"].controllerButtons.push_back(XINPUT_GAMEPAD_DPAD_UP);
	keyBinds_["MoveBack"].controllerButtons.push_back(XINPUT_GAMEPAD_DPAD_DOWN);
	keyBinds_["Stript"].controllerButtons.push_back(XINPUT_GAMEPAD_A);
	keyBinds_["Shot"].controllerButtons.push_back(XINPUT_GAMEPAD_A);
	keyBinds_["Menu"].controllerButtons.push_back(XINPUT_GAMEPAD_START);
}

void KeyBindConfig::Update() {
	bool isControll = false;
	for (auto& [actionName, inputDevice] : keyBinds_) {
		isControll |= IsPress(actionName);
	}
	if (isControll) {
		noControllTimer_ = 0.0f;
	} else {
		noControllTimer_ += 0.016f;
	}
}

bool KeyBindConfig::IsPress(const std::string& actionName) {
	for (const auto& keyBind : keyBinds_[actionName].keyboardKeys) {
		if (Input::IsPushKey(keyBind)) {
			return true;
		}
	}
	for (const auto& buttonBind : keyBinds_[actionName].controllerButtons) {
		if (Input::IsControllerPressButton(buttonBind,0)) {
			return true;
		}
	}
	return false;
}

bool KeyBindConfig::IsTrigger(const std::string& actionName) {
	for (const auto& keyBind : keyBinds_[actionName].keyboardKeys) {
		if (Input::IsTriggerKey(keyBind)) {
			return true;
		}
	}
	for (const auto& buttonBind : keyBinds_[actionName].controllerButtons) {
		if (Input::IsControllerTriggerButton(buttonBind, 0)) {
			return true;
		}
	}
    return false;
}

bool KeyBindConfig::IsRelease(const std::string& actionName) {
	for (const auto& keyBind : keyBinds_[actionName].keyboardKeys) {
		if (Input::IsReleaseKey(keyBind)) {
			return true;
		}
	}
	return false;
}
