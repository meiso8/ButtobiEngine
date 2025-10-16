#pragma once
#include"SceneManager.h"

class TitleScene :public SceneManager {

public:
    void Initialize()override;
    void Update()override;
    void Draw()override;
	bool GetIsEndTitle();
private:
	bool isEndTitle_ = false;
	enum class AnimationPhase {
		JInOutP,
		Korokoro,
	} animationPhase_ = AnimationPhase::JInOutP;

	enum class JuiceInOutPhase {

		InJuice,
		RotateJuice,
		OutJuice,
	};

	enum class GameOption {

		GameStart,
		Option,
		CloseGame,
		None,
	};

	enum class Option {

		BGM,
		SE,
		Back,

	};

	enum class CloseGame {

		YES,
		NO,

	};

	GameOption selectGameoption = GameOption::GameStart;
	GameOption gameOption_ = GameOption::None;

	Option option_ = Option::BGM;
	CloseGame closeGame_ = CloseGame::NO;
	void Move();
};
