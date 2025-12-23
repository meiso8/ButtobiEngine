#pragma once
#include"Object3d.h"
#include"Quaternion/Quaternion.h"
#include"Animation/Animation.h"
struct ModelData;
class Model;
class AnimationObject3d :public Object3d
{
public:
    AnimationObject3d();

    void InitTime();
    void SetMeshAndData(Model* model);

    //オーバーライド
    void Update()override;
    void Draw(Camera& camera, const BlendMode& blendMode = BlendMode::kBlendModeNormal, const CullMode& cullMode = CullMode::kCullModeBack)override;

private:

    void UpdateAnimation();
    float animationTime_ = 0.0f;
    Quaternion rotate_ = { 0.0f };
    Animation animation;
    ModelData* modelData_ = nullptr;
    Matrix4x4 worldMatrix_ = { 0.0f };
};

