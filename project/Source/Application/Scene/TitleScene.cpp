#include "TitleScene.h"
#include "Input.h"

void TitleScene::Initialize(){
	
	isEndTitle_ = false; }

void TitleScene::Update(){

	if (Input::IsTriggerKey(DIK_SPACE)) {
		isEndTitle_ = true;
	}





}

void TitleScene::Draw(){





}

bool TitleScene::GetIsEndTitle(){ return isEndTitle_; }