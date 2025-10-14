#pragma once
#include"SceneManager.h"
#include"SamplePlayer.h"
#include"Transform.h"
#include"WorldTransform.h"
#include"Cube.h"
#include"Particle/Particle.h"
#include"Quad.h"
#include"SphereMesh.h"

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

    Transform cameraTransform_;

    //スプライト
    std::vector<Sprite*>sprites_;

    ////パーティクル
    ParticleMesh particle_;
    //立方体
    std::vector<Cube>cube_;
    std::unique_ptr<SphereMesh> sphereMesh_ = nullptr;
    WorldTransform worldTransformChild_;

    WorldTransform worldTransformParent_;
    //プレイヤー
    std::unique_ptr<SamplePlayer>samplePlayer_ = nullptr;
    QuadMesh quad_;
    Vector2 scale_;
    Vector2 uvMoveVal_;
};

