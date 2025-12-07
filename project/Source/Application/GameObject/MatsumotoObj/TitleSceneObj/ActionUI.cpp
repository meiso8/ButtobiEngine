#include "ActionUI.h"
#include "MatsumotoObj/KeyBindConfig.h"
#include "MatsumotoObj/MY_Utility.h"
#include "MatsumotoObj/GameSceneObj/FloorGamePlayer.h"

ActionUI::ActionUI(FloorGamePlayer* player) {
	player_ = player;

	moveSprite_ = std::make_unique<Sprite>();
	actionSprite_ = std::make_unique<Sprite>();


	moveSprite_->Create(Texture::ACTION_UI_MOVE, { 128.0f,128.0f }, { 1.0f,1.0f,1.0f,1.0f });
	actionSprite_->Create(Texture::ACTION_UI_STRIP, { 128.0f,128.0f }, { 1.0f,1.0f,1.0f,1.0f });

	moveSprite_->SetSize({ 256.0f,64.0f });
	actionSprite_->SetSize({ 256.0f,64.0f });



}

void ActionUI::Initialize() {
	moveUiY = 150.0f;
	stripUiY = 230.0f;


	moveSprite_->SetPosition({ 0.0f,moveUiY });
	actionSprite_->SetPosition({ 0.0f,stripUiY });

	moveSprite_->SetAnchorPoint({ 0.0f,0.5f });
	actionSprite_->SetAnchorPoint({ 0.0f,0.5f });

	isHide_ = false;
}

void ActionUI::Update() {
	if (player_->isMove_) {
		moveSprite_->GetScale().x = MY_Utility::SimpleEaseIn(moveSprite_->GetScale().x, 0.8f, 0.1f);
	} else {
		moveSprite_->GetScale().x = MY_Utility::SimpleEaseIn(moveSprite_->GetScale().x, 1.0f, 0.5f);
	}

	if (player_->isReqestStript_ || player_->isReqestShot_) {
		actionSprite_->GetScale().x = 0.8f;
	} else {
		actionSprite_->GetScale().x = MY_Utility::SimpleEaseIn(actionSprite_->GetScale().x, 1.0f, 0.1f);
	}

	if (isHide_) {
		moveSprite_->SetPosition(MY_Utility::SimpleEaseIn(moveSprite_->GetPosition(),{-256.0f,moveUiY},0.3f));
		actionSprite_->SetPosition(MY_Utility::SimpleEaseIn(actionSprite_->GetPosition(), { -256.0f,stripUiY }, 0.3f));
		
	} else {
		moveSprite_->SetPosition(MY_Utility::SimpleEaseIn(moveSprite_->GetPosition(), { 0.0f,moveUiY }, 0.3f));
		actionSprite_->SetPosition(MY_Utility::SimpleEaseIn(actionSprite_->GetPosition(), { 0.0f,stripUiY }, 0.3f));
		
	}
	
	if (player_->isStriptting_) {
		actionSprite_->SetTexture(Texture::ACTION_UI_SHOT);
	} else {
		actionSprite_->SetTexture(Texture::ACTION_UI_STRIP);
	}

	moveSprite_->Update();
	actionSprite_->Update();

	//shotSprite_->Update();
}

void ActionUI::Draw() {

	Sprite::PreDraw();
	//moveSprite_->PreDraw();
	//actionSprite_->PreDraw();
	//menuSprite_->PreDraw();
	//shotSprite_->PreDraw();

	moveSprite_->Draw();
	actionSprite_->Draw();


	//shotSprite_->Draw();
}
