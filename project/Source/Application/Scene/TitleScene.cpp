#include "TitleScene.h"
#include "Input.h"

void TitleScene::Initialize(){
	
	isEndTitle_ = false; }

void TitleScene::Update(){

	if (Input::IsTriggerKey(DIK_SPACE)) {
		isEndTitle_ = true;
	}





}


void TitleScene::Move() {

		switch (gameOption_) {
		case TitleScene::GameOption::GameStart:

			if (Input::IsTriggerKey(DIK_SPACE)) {

				isEndTitle_ = true;
			}

			break;
		case TitleScene::GameOption::Option:

			switch (option_) {
			case TitleScene::Option::BGM:

				// オプション
			    if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

					if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

						option_ = Option::Back;

					} else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

						option_ = Option::SE;
					}
				}
				break;
			case TitleScene::Option::SE:
			    if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {
				    if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

						option_ = Option::BGM;

					} else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

						option_ = Option::Back;
					}
				}
				break;
			case TitleScene::Option::Back:
			    if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {
				    if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

						option_ = Option::SE;

					} else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

						option_ = Option::BGM;
					}
				}
			    if (Input::IsTriggerKey(DIK_SPACE)) {

					selectGameoption = GameOption::GameStart;
					gameOption_ = GameOption::None;
				}
				break;
			default:
				break;
			}

			break;
		case TitleScene::GameOption::CloseGame:
			switch (closeGame_) {
			case TitleScene::CloseGame::YES:

				if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

					closeGame_ = CloseGame::NO;
				}
			    if (Input::IsTriggerKey(DIK_SPACE)) {
					// ゲームを終了する
				}

				break;
			case TitleScene::CloseGame::NO:

				if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

					closeGame_ = CloseGame::YES;
				}

				if (Input::IsTriggerKey(DIK_SPACE)) {
					selectGameoption = GameOption::GameStart;
					gameOption_ = GameOption::None;
				}

				break;
			default:
				break;
			}
			break;
		case TitleScene::GameOption::None:
		    if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP) || Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {
				if (selectGameoption == GameOption::GameStart) {
				    if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

						selectGameoption = GameOption::CloseGame;

					} else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

						selectGameoption = GameOption::Option;
					}
					break;
				} else if (selectGameoption == GameOption::Option) {
				    if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

						selectGameoption = GameOption::GameStart;

					} else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

						selectGameoption = GameOption::CloseGame;
					}
					break;
				} else if (selectGameoption == GameOption::CloseGame) {
				    if (Input::IsTriggerKey(DIK_W) || Input::IsTriggerKey(DIK_UP)) {

						selectGameoption = GameOption::Option;

					} else if (Input::IsTriggerKey(DIK_S) || Input::IsTriggerKey(DIK_DOWN)) {

						selectGameoption = GameOption::GameStart;
					}
					break;
				}
			}

			if (Input::IsTriggerKey(DIK_SPACE)) {

				gameOption_ = selectGameoption;
			}

			break;
		default:
			break;
		}

	
	}





void TitleScene::Draw(){





}

bool TitleScene::GetIsEndTitle(){ return isEndTitle_; }