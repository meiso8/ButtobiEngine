#pragma once
#include"SceneManager.h"
class GameOverScene :public SceneManager {

	private:
	public:
		void Initialize()override;
		void Update()override;
		void Draw()override;
		bool GetIsEndScene()override;


};