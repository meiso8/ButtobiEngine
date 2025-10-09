#pragma once
#include"SceneManager.h"
#include"SamplePlayer.h"
#include"Transform.h"
#include"WorldTransform.h"
#include"Cube.h"
#include"Particle/Particle.h"
#include"Camera/Camera.h"
#include"Camera/DebugCamera.h"

class Sprite;


class SampleScene :public  SceneManager
{
public:
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
    ~SampleScene();
private:
    //ライトタイプ
    uint32_t lightType_ = 0;
    //ブレンドモード
    uint32_t blendMode_ = 0;

    //カメラ
    std::unique_ptr<Camera> camera_ = nullptr;
    Transform cameraTransform_;
    bool isDebugCameraActive_ = false;
    std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
    Camera* currentCamera = nullptr;

    //スプライト
    std::vector<Sprite*>sprites_;

    ////パーティクル
    //Particle particle_;
    //立方体
    std::vector<Cube>cube_;
    WorldTransform cubeWorldTransform_;
    //プレイヤー
    std::unique_ptr<SamplePlayer>samplePlayer_ = nullptr;

};

