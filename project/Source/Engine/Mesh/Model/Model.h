#include"MeshCommon.h"
#include"ModelManager.h"
#include"ModelData.h"

class Model :public MeshCommon
{
public:
    ~Model();
    void Create(const ModelManager::MODEL_HANDLE& modelHandle);
    void Draw(ID3D12GraphicsCommandList* commandList)override;
    void UpdateUV();
    Transform& GetUVTransform() { return uvTransform_; }
private:
    void CreateVertex()override;
    void CreateUV();
private:
    const ModelData* modelData_;
    Transform uvTransform_ = { 0.0f };
    Matrix4x4 uvTransformMatrix_{ 0.0f };
};
