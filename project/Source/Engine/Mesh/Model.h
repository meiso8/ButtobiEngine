#include"MeshCommon.h"
#include"ModelManager.h"

class Model :public MeshCommon
{
public:

    void Create(const ModelManager::MODEL_HANDLE& modelHandle);
    void PreDraw(const BlendMode& type = BlendMode::kBlendModeNormal)override;
    void Draw(Camera& camera, const Matrix4x4& worldMatrix, const uint32_t lightType = MaterialResource::LIGHTTYPE::NONE)override;

    void UpdateUV();
    Transform& GetUVTransform() { return uvTransform_; }
private:
    void CreateVertex()override;
    void CreateUV();
private:
    const ModelData* modelData_;
    Transform uvTransform_;
    Matrix4x4 uvTransformMatrix_{};


};
