#pragma once
#include"SceneManager.h"
#include"WorldTransform.h"
#include "Sprite.h"
#include<memory>
#include<array>
class FlashParticle;
class AppleSceneChange;
class QuadMesh;
class Model;
class TitleScene :public SceneManager {

    enum class TitleString {
        bu,//ぶ
        tt, // っ
        to,  // と
        bi,  // び
        mi,  // み
        kk, // っ
        ku, // く
        su, // す
        num, // 要素数

    };

    enum class AnimationPhase {
        JInOutP,
        Korokoro,
    }animationPhase_ = AnimationPhase::JInOutP;

    enum class JuiceInOutPhase {

        InJuice,
        RotateJuice,
        OutJuice,
    };

    enum class GameOption {

        GameStart,
        Option,
        CloseGame,
        None,
    };

    enum class Option {

        BGM,
        SE,
        Back,

    };

    enum class CloseGame {

        YES,
        NO,

    };


    GameOption selectGameoption_ = GameOption::GameStart;
    GameOption gameOption_ = GameOption::None;

    Option option_ = Option::BGM;
    CloseGame closeGame_ = CloseGame::NO;

    bool IsAnimationEnd = false;
    bool iscameraTranslateEnd = false;
    //位置
    Vector3 titleStringPosition_[8] = {
        {-3.5f,1.0f,0.0f}, // ぶ
        {-2.5f,1.0f,0.0f}, // っ
        {-1.5f,1.0f,0.0f}, // と
        {-0.5f,1.0f,0.0f}, // び
        { 0.5f,1.0f,0.0f}, // み
        { 1.5f,1.0f,0.0f}, // っ
        { 2.5f,1.0f,0.0f}, // く
        { 3.5f,1.0f,0.0f}, // す
    };

    Vector3 stringInStartPosition[8] = {
        {0.0f,29.0f,0.0f}, // ぶ
        {0.0f,27.0f,0.0f}, // っ
        {0.0f,25.0f,0.0f}, // と
        {0.0f,23.0f,0.0f}, // び
        { 0.0f,21.0f,0.0f}, // み
        { 0.0f,19.0f,0.0f}, // っ
        { 0.0f,17.0f,0.0f}, // く
        { 0.0f,15.0f,0.0f}, // す
    };

    Vector3 stringInEndPosition[8] = {
        {0.0f,4.0f,0.0f}, // ぶ
        {0.0f,3.0f,0.0f}, // っ
        {0.0f,2.0f,0.0f}, // と
        {0.0f,1.0f,0.0f}, // び
        { 0.0f,0.0f,0.0f}, // み
        { 0.0f,-1.0f,0.0f}, // っ
        { 0.0f,-2.0f,0.0f}, // く
        { 0.0f,-3.0f,0.0f}, // す
    };

    Vector3 stringOutEndPosition[8] = {
        {-22.0f, -2.0f, 0.0f}, // ぶ
        {-20.0f, -2.0f, 0.0f}, // っ
        {-18.0f, -2.0f, 0.0f}, // と
        {-16.0f, -2.0f, 0.0f}, // び
        {-14.0f, -2.0f, 0.0f}, // み
        {-12.0f, -2.0f, 0.0f}, // っ
        {-10.0f, -2.0f, 0.0f}, // く
        {-8.0f, -2.0f, 0.0f}, // す

    };
    Vector3 titleStringSize = { 1,1,1 };

    Vector3 cameraPositionStart = { 0, 0, -15.0f };

    Vector3 cameraPositionEnd = { -15.0f, 0, -15.0f };

    int prerandum{};
    int randum{};

    Model* titleStringModel[8] = { nullptr };
    WorldTransform titleStringWorldTransform[8];



    Model* juiceCupModel = nullptr;
    WorldTransform juiceCupWorldTransform;

    std::array<std::unique_ptr<Model>, 6> tableModels;
    std::array < WorldTransform, 6>tableWorldTransforms;

    std::unique_ptr<QuadMesh> quad_ = nullptr;
    WorldTransform quadWorldTransform_;

    Model* appleModel = nullptr;
    WorldTransform appleWorldTransform;

    Model* backGroundModel = nullptr;
    WorldTransform backGroundWorldTransform;

    std::unique_ptr<Sprite> spaceSprite_ = nullptr;
    uint32_t spaceTExtureHandle_ = 0;

    Vector3 EaseIn(float t, const Vector3& start, const Vector3& end) {
        // イージング関数（加速）
        float easedT = t * t; // 二次関数で加速
        return start + (end - start) * easedT;
    }

    // スカラー版 easeInOutQuart
    float easeInOutQuart(float t) { return (t < 0.5f) ? 8.0f * t * t * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 4.0f) / 2.0f; }

    // Vector3 用の補間 (start→end を t で easeInOutQuart補間)
    Vector3 easeInOutQuart(const Vector3& start, const Vector3& end, float t);
    float stringAnimationTimer = 0.0f;

    JuiceInOutPhase jInOutPhase_;
    //パーティクル
    std::unique_ptr <FlashParticle> flashParticle_ = nullptr;

    std::unique_ptr <AppleSceneChange> appleSceneChange_ = nullptr;


public:
    TitleScene();
    ~TitleScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Move();
    void StringInOutJuiceAnimation();
    void KorokoroAnimation();
    void LoopAnimation();
    void FlashParticlePop();
    void Debug()override;
};

//class TitleScene :public SceneManager {
//
//public:
//    void Initialize()override;
//    void Update()override;
//    void Draw()override;
//    bool GetIsEndScene()override;
//private:
//
//	enum class AnimationPhase {
//		JInOutP,
//		Korokoro,
//	} animationPhase_ = AnimationPhase::JInOutP;
//
//	enum class JuiceInOutPhase {
//
//		InJuice,
//		RotateJuice,
//		OutJuice,
//	};
//
//	enum class GameOption {
//
//		GameStart,
//		Option,
//		CloseGame,
//		None,
//	};
//
//	enum class Option {
//
//		BGM,
//		SE,
//		Back,
//
//	};
//
//	enum class CloseGame {
//
//		YES,
//		NO,
//
//	};
//
//	GameOption selectGameoption = GameOption::GameStart;
//	GameOption gameOption_ = GameOption::None;
//
//	Option option_ = Option::BGM;
//	CloseGame closeGame_ = CloseGame::NO;
//	void Move();
//	~TitleScene();
//};
