#pragma once
#include"SceneManager.h"
class GameClearScene:public SceneManager {

	private:
	bool isEndGameClear_ = false;
	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
	    bool GetIsEndGameClear();





};
