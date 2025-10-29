#include"MeshCommon.h"
#include"ModelManager.h"

class Model :public MeshCommon
{
public:
    ~Model();
    void Create(const ModelManager::MODEL_HANDLE& modelHandle);
    void PreDraw(const BlendMode& type = BlendMode::kBlendModeNormal,const CullMode& cullMode = CullMode::kCullModeBack)override;
    void Draw(Camera& camera, const Matrix4x4& worldMatrix, const uint32_t lightType = MaterialResource::LIGHTTYPE::NONE)override;

    void UpdateUV();
    Transform& GetUVTransform() { return uvTransform_; }
private:
    void CreateVertex()override;
    void CreateUV();
private:
    const ModelData* modelData_;
    Transform uvTransform_ = {0.0f};
    Matrix4x4 uvTransformMatrix_{0.0f};


};
