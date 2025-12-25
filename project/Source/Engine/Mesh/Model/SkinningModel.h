#pragma once

#include"Bone.h"
#include"SkinCluster.h"
#include"MeshCommon.h"
#include<memory>
class Model;
class SkinningModel :public MeshCommon
{
public:
    SkinningModel();
    ~SkinningModel();
    void CreateDatas(Model* model);
    void PreDraw(ID3D12GraphicsCommandList* commandList, const BlendMode& blendMode = BlendMode::kBlendModeNormal, const CullMode& cullMode = CullMode::kCullModeBack)override;
    void Draw(ID3D12GraphicsCommandList* commandList)override;
    Skeleton* GetSkeleton() {
        return skeleton_.get();
    };
    SkinCluster* GetSkinCluster() {
        return skinCluster_.get();
    };
    /// @brief テクスチャをモデルオリジナルのものに戻す
    void ResetTextureHandle();
    ModelData* GetModelData() {
        return modelData_;
    }

private:
    void CreateVertex()override;
    SkinningModel* skinningModel_ = nullptr;
    std::unique_ptr< Skeleton> skeleton_;
    std::unique_ptr< SkinCluster> skinCluster_;
    ModelData* modelData_ = nullptr;
};

