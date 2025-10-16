#include "TitleScene.h"
#include "Input.h"

void TitleScene::Initialize(){
	
	isEndScene_ = false; }

void TitleScene::Update(){

	if (Input::IsTriggerKey(DIK_SPACE)) {
		isEndScene_ = true;
	}





}

void TitleScene::Draw(){





}

bool TitleScene::GetIsEndScene(){ return isEndScene_; }