#include"WorldTransform.h"
#include"Quad.h"
#include<memory>
struct OBB;
class OBBRenderer;
class Camera;

class Arrow {
private:
    // ワールド変換データ
    WorldTransform worldTransform_;
    WorldTransform worldTransformParent_;
    std::unique_ptr< QuadMesh> quad_;
#ifdef _DEBUG
    //OBBのデバック描画
    std::unique_ptr<OBBRenderer> obbRenderer_ = nullptr;
#endif // _DEBUG

    //キックOBB
    std::unique_ptr < OBB >kickObb_;
public:
    Arrow();
    /// @brief 初期化
    void Initialize();
    /// @brief 更新
    void Update(const Vector3& position, const float& scaleZ, const float& rotateY);
    /// @brief 描画
    void Draw(Camera& camera);
    ~Arrow();
    Vector3 GetWorldPosition()const;
    /// @brief 蹴る場所のOBBの取得
/// @return OBB
    OBB GetKickAreaOBB();


    float GetScaleZ() { return worldTransform_.scale_.z; }
};