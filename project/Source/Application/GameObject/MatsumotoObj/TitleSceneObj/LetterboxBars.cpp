#include "LetterboxBars.h"
#include "MatsumotoObj/MY_Utility.h"
LetterboxBars::LetterboxBars() {
	topBar_ = std::make_unique<Sprite>();
	bottomBar_ = std::make_unique<Sprite>();
	topBar_->Create(Texture::WHITE_1X1, { 640.0f,50.0f }, { 0.0f,0.0f,0.0f,1.0f });
	bottomBar_->Create(Texture::WHITE_1X1, { 640.0f,710.0f }, { 0.0f,0.0f,0.0f,1.0f });
	topBar_->SetSize({ 1280.0f,100.0f });
	bottomBar_->SetSize({ 1280.0f,100.0f });

}

void LetterboxBars::Initialize() {
	topBar_->SetPosition({ 640.0f,50.0f });
	topBar_->SetAnchorPoint({ 0.5f,0.5f });
	topBar_->Update();
	bottomBar_->SetPosition({ 640.0f,710.0f });
	bottomBar_->SetAnchorPoint({ 0.5f,0.5f });
	bottomBar_->Update();

	isOpen_ = false;
	offsetY_ = -50.0f;
}

void LetterboxBars::Update() {
	topBar_->SetPosition({ 640.0f,offsetY_ });
	bottomBar_->SetPosition({ 640.0f,720.0f - offsetY_ });

	if (isOpen_) {
		offsetY_ = MY_Utility::SimpleEaseIn(offsetY_, 50.0f, 0.1f);
	} else {
		offsetY_ = MY_Utility::SimpleEaseIn(offsetY_, -50.0f, 0.1f);
	}
	
	topBar_->Update();
	bottomBar_->Update();
}

void LetterboxBars::Draw() {
	topBar_->PreDraw();
	topBar_->Draw();
	bottomBar_->PreDraw();
	bottomBar_->Draw();
}
