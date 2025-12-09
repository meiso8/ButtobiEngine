#include "GameOverEvent.h"
#include "MatsumotoObj/GameSceneObj/FloorGamePlayer.h"
#include "MatsumotoObj/KeyBindConfig.h"
#include "MatsumotoObj/MY_Utility.h"
#include"Sound.h"
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

	gameOverStringSprite_ = std::make_unique<Sprite>();
	gameOverStringSprite_->Create(Texture::GAMEOVER_STRING, { static_cast<float>(Window::GetClientWidth()) * 0.5f,-200.0f }, { 1.0f,1.0f,1.0f,1.0f });

	tips1Sprite_ = std::make_unique<Sprite>();
	tips1Sprite_->Create(Texture::GAMEOVER_TIPS_1, { static_cast<float>(Window::GetClientWidth()) * 0.7f , 720.0f+450.0f }, { 1.0f,1.0f,1.0f,1.0f });
}

GameOverEvent::~GameOverEvent() {
}

void GameOverEvent::Initialize() {
	isGameOver_ = false;
	isRetrySelected_ = true;
	isReqestedAction_ = false;
	timer_ = 0.0f;

	retrySprite_->SetPosition({ -500.0f, static_cast<float>(Window::GetClientHeight()) * 0.25f+ kYOffset });
	backToTitleSprite_->SetPosition({ static_cast<float>(Window::GetClientWidth()) + 500.0f, static_cast<float>(Window::GetClientHeight()) * 0.25f + kYOffset * 1.5f });
	gameOverStringSprite_->SetPosition({ static_cast<float>(Window::GetClientWidth()) * 0.5f, -200.0f });
	tips1Sprite_->SetPosition({ static_cast<float>(Window::GetClientWidth()) * 0.7f , 720.0f + 450.0f });

	gameOverStringSprite_->SetAnchorPoint({ 0.5f,0.5f });
	tips1Sprite_->SetAnchorPoint({ 0.5f,0.5f });
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
			Vector2{ static_cast<float>(Window::GetClientWidth()) * 0.45f - kXOffset,
				static_cast<float>(Window::GetClientHeight()) * 0.5f + kYOffset * 0.5f },
			0.1f));
		backToTitleSprite_->SetPosition(MY_Utility::SimpleEaseIn(
			backToTitleSprite_->GetPosition(),
			Vector2{ static_cast<float>(Window::GetClientWidth()) * 0.45f - kXOffset,
				static_cast<float>(Window::GetClientHeight()) * 0.5f + kYOffset},
			0.1f));

		gameOverStringSprite_->SetPosition(MY_Utility::SimpleEaseIn(
			gameOverStringSprite_->GetPosition(),
			Vector2{ static_cast<float>(Window::GetClientWidth()) * 0.5f,
				200.0f },
			0.1f));
		tips1Sprite_->SetPosition(MY_Utility::SimpleEaseIn(
			tips1Sprite_->GetPosition(),
			Vector2{ static_cast<float>(Window::GetClientWidth()) * 0.7f ,
				450.0f },
			0.1f));

		gameOverStringSprite_->SetScale({ 2.0f + sinf(timer_*2.5f) * 0.05f,2.0f + sinf(timer_ * 2.5f) * 0.05f });
	}

	if (isGameOver_) {
		timer_ += 0.016f;
	}

	KeyBindConfig& key = KeyBindConfig::Instance();
	if (key.IsTrigger("MoveForward")) {
		isRetrySelected_ = true;
		Sound::PlaySE(Sound::kMoveCursor,1.0f);
	}
	if (key.IsTrigger("MoveBack")) {
		isRetrySelected_ = false;
		Sound::PlaySE(Sound::kMoveCursor,1.0f);
	}
	if (key.IsTrigger("Shot")) {
		Sound::PlaySE(Sound::kDecision,1.0f);
		if (timer_ > 1.0f) {
			isReqestedAction_ = true;
		}
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
	gameOverStringSprite_->Update();
	tips1Sprite_->Update();
}

void GameOverEvent::Draw() {
	retrySprite_->PreDraw();
	backToTitleSprite_->PreDraw();
	gameOverStringSprite_->PreDraw();
	tips1Sprite_->PreDraw();

	retrySprite_->Draw();
	backToTitleSprite_->Draw();
	gameOverStringSprite_->Draw();
	tips1Sprite_->Draw();
}
