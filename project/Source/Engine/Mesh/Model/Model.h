#include"MeshCommon.h"
#include"ModelManager.h"
#include"ModelData.h"
#include<memory>

class Model :public MeshCommon
{
public:
    ~Model();
    void Create();
    void SetModelData(std::unique_ptr<ModelData> modelData) {
        modelData_ = std::move(modelData);
    }
    ModelData* GetModelData() {
        return modelData_.get();
    }
    void Draw(ID3D12GraphicsCommandList* commandList)override;
    void UpdateUV();
    Transform& GetUVTransform() { return uvTransform_; }

    void ResetTextureHandle() {
        textureHandle_ = modelData_->material.textureSrvIndex;
    };
private:
    void CreateVertex()override;
    void CreateUV();
private:
    std::unique_ptr<ModelData> modelData_ = nullptr;
    Transform uvTransform_ = { 0.0f };
    Matrix4x4 uvTransformMatrix_{ 0.0f };
};
