#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <stdint.h>
#include <unordered_map>
#include <vector>
#include <string>

struct InputDevice {
	std::vector<uint8_t> keyboardKeys;
	std::vector<UINT16> controllerButtons;
};

class KeyBindConfig
{
public:
	static KeyBindConfig& Instance() {
		static KeyBindConfig instance;
		return instance;
	}

	KeyBindConfig(const KeyBindConfig&) = delete;
	KeyBindConfig& operator=(const KeyBindConfig&) = delete;

	bool IsPress(const std::string& actionName);
	bool IsTrigger(const std::string& actionName);
	bool IsRelease(const std::string& actionName);

private:
	std::unordered_map<std::string, InputDevice> keyBinds_;
	KeyBindConfig();
	~KeyBindConfig() = default;
};