#pragma once
#include"SceneManager.h"
class GameOverScene :public SceneManager {

	private:
	bool isEndGameOver_ = false;


	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
	    bool GetIsEndGameOver();


};