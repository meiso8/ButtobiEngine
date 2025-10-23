#include"WorldTransform.h"
#include"Quad.h"
#include<memory>

class Camera;


class Arrow {
public:
    Arrow();
    /// @brief 初期化
    void Initialize();
    /// @brief 更新
    void Update(const Vector3& position, const float& scaleZ, const float& rotateY);
    /// @brief 描画
    void Draw(Camera& camera);
    ~Arrow();
private:
    WorldTransform worldTransformParent_;
    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル

    std::unique_ptr< QuadMesh> quad_;
};