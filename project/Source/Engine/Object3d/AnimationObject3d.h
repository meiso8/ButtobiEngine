#pragma once
#include"Object3d.h"
#include"Quaternion/Quaternion.h"
#include"Animation/Animation.h"

class DebugBone;
struct SkinCluster;
class SkinningModel;
struct ModelData;

class AnimationObject3d :public Object3d
{
public:
    AnimationObject3d();
    void Initialize()override;
    void InitTime();
    void SetMeshAndData(SkinningModel* skinningModel);
    void SetAnimation(Model* model);
    void SetModel(Model* model);
    void SetTextureHandle(const Texture::TEXTURE_HANDLE& handle)override;
    //オーバーライド
    void Update()override;
    void UpdateAniTimer(const bool& isLoop = true);

    void Draw(Camera& camera, const BlendMode& blendMode = BlendMode::kBlendModeNormal, const CullMode& cullMode = CullMode::kCullModeBack)override;
private:
    void UpdateAnimation();
    float animationTime_ = 0.0f;
    Animation animation_;
    Matrix4x4 worldMatrix_ = { 0.0f };
    bool isSkinning_ = true;
    SkinningModel* skinningModel_ = nullptr;
#ifdef _DEBUG
    std::unique_ptr< DebugBone> debugBone_;
#endif
};

