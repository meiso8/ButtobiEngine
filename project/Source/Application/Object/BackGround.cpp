#include "BackGround.h"

#include"Camera/Camera.h"

void BackGround::Initialize()
{
    // モデルの生成 OBJからの生成
    backGroundModel_ = std::make_unique<Model>();
    backGroundModel_->Create(ModelManager::BACK_GROUND);

    // ワールド変換の初期化
    backGroundWorldTransform_.Initialize();
    WorldTransformUpdate(backGroundWorldTransform_);
}

void BackGround::Draw(Camera& camera)
{
    //背景
    backGroundModel_->PreDraw();
    backGroundModel_->Draw(camera, backGroundWorldTransform_.matWorld_);

}
