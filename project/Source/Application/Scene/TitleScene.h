#pragma once
#include"SceneManager.h"
#include"WorldTransform.h"
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

    Vector3 cameraPositionStart = { 0, 0, -30 };

    Vector3 cameraPositionEnd = { -15.0f, 0, -30 };

    Model* titleStringModel[8] = { nullptr };
    WorldTransform titleStringWorldTransform[8];

    Model* juiceCupModel = nullptr;
    WorldTransform juiceCupWorldTransform;

    Model* tableModel = nullptr;
    WorldTransform tableWorldTransform;

    Model* appleModel = nullptr;
    WorldTransform appleWorldTransform;

    Vector3 EaseIn(float t, const Vector3& start, const Vector3& end) {
        // イージング関数（加速）
        float easedT = t * t; // 二次関数で加速
        return start + (end - start) * easedT;
    }

    float stringAnimationTimer = 0.0f;

    JuiceInOutPhase jInOutPhase_;
public:
    TitleScene();
    ~TitleScene();
    void Initialize()override;
    void Update()override;
    void Draw()override;
    bool GetIsEndScene()override;
    void Move();
    void StringInOutJuiceAnimation();
    void KorokoroAnimation();
    void LoopAnimation();
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
