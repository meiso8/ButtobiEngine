#include "GameOverEvent.h"
#include "MatsumotoObj/GameSceneObj/FloorGamePlayer.h"
#include "MatsumotoObj/KeyBindConfig.h"
#include "MatsumotoObj/MY_Utility.h"

namespace {
	const float kXOffset = 200.0f;
	const float kYOffset = 200.0f;
}

GameOverEvent::GameOverEvent(FloorGamePlayer* player) {
	player_ = player;
	retrySprite_ = std::make_unique<Sprite>();
	backToTitleSprite_ = std::make_unique<Sprite>();
	retrySprite_->Create(Texture::BUTTON_RETRY, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
	backToTitleSprite_->Create(Texture::BUTTON_BACK_TO_TITL, { 0.0f,0.0f }, { 1.0f,1.0f,1.0f,1.0f });
}

GameOverEvent::~GameOverEvent() {
}

void GameOverEvent::Initialize() {
	isGameOver_ = false;
	isRetrySelected_ = true;
	isReqestedAction_ = false;
	timer_ = 0.0f;

	retrySprite_->SetPosition({ -500.0f, static_cast<float>(Window::GetClientHeight()) * 0.5f+ kYOffset });
	backToTitleSprite_->SetPosition({ static_cast<float>(Window::GetClientWidth()) + 500.0f, static_cast<float>(Window::GetClientHeight()) * 0.5f + kYOffset });

	retrySprite_->SetAnchorPoint({ 0.5f,0.5f });
	backToTitleSprite_->SetAnchorPoint({ 0.5f,0.5f });
}

void GameOverEvent::Update() {
	if (player_->GetDamageStruct().hps.hp <= 0 && !isGameOver_) {
		isGameOver_ = true;
	}

	if (!isGameOver_ || isReqestedAction_) {
		retrySprite_->SetPosition(MY_Utility::SimpleEaseIn(
			retrySprite_->GetPosition(),
			Vector2{-500.0f,
				static_cast<float>(Window::GetClientHeight()) * 0.5f + kYOffset },
			0.1f));
		backToTitleSprite_->SetPosition(MY_Utility::SimpleEaseIn(
			backToTitleSprite_->GetPosition(),
			Vector2{ static_cast<float>(Window::GetClientWidth()) +500.0f,
				static_cast<float>(Window::GetClientHeight()) * 0.5f + kYOffset },
			0.1f));
		return;
	} else {
		retrySprite_->SetPosition(MY_Utility::SimpleEaseIn(
			retrySprite_->GetPosition(),
			Vector2{ static_cast<float>(Window::GetClientWidth()) * 0.5f - kXOffset,
				static_cast<float>(Window::GetClientHeight()) * 0.5f + kYOffset },
			0.1f));
		backToTitleSprite_->SetPosition(MY_Utility::SimpleEaseIn(
			backToTitleSprite_->GetPosition(),
			Vector2{ static_cast<float>(Window::GetClientWidth()) * 0.5f + kXOffset,
				static_cast<float>(Window::GetClientHeight()) * 0.5f + kYOffset },
			0.1f));
	}

	if (isGameOver_) {
		timer_ += 0.016f;
	}

	KeyBindConfig& key = KeyBindConfig::Instance();
	if (key.IsTrigger("MoveLeft")) {
		isRetrySelected_ = true;
	}
	if (key.IsTrigger("MoveRight")) {
		isRetrySelected_ = false;
	}
	if (key.IsTrigger("Shot")) {
		isReqestedAction_ = true;
	}

	if (isRetrySelected_) {
		retrySprite_->SetScale({ 1.5f + sinf(timer_) * 0.1f,1.5f + sinf(timer_) * 0.1f });
		backToTitleSprite_->SetScale({ 1.0f,1.0f });
	} else {
		retrySprite_->SetScale({ 1.0f,1.0f });
		backToTitleSprite_->SetScale({ 1.5f + sinf(timer_) * 0.1f,1.5f + sinf(timer_) * 0.1f });
	}

	retrySprite_->Update();
	backToTitleSprite_->Update();
}

void GameOverEvent::Draw() {
	retrySprite_->PreDraw();
	backToTitleSprite_->PreDraw();
	retrySprite_->Draw();
	backToTitleSprite_->Draw();
}
