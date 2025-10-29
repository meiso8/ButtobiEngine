#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"
#include <array>
#include <memory>
#include <string>

struct OBB {
    Vector3 center;		// 中心点
    Vector3 axis[3];	// 各軸の単位ベクトル
    Vector3 halfSizes;	// 各軸方向の半分の長さ
};

/// @brief 各軸の単位ベクトルの作成
/// @param rotate 回転角
/// @param obb OBB
void MakeAxis(const Vector3& rotate, OBB& obb);

/// @brief OBB行列の作成
/// @param obb OBB
/// @return OBB行列
Matrix4x4 MakeOBBMatrix(const OBB &obb);

#ifdef _DEBUG
/// @brief 編集
/// @param label ラベル
/// @param rotate 回転角
/// @param obb OBB
void EditOBB(const std::string& label, Vector3& rotate, OBB& obb);
#endif // _DEBUG

class LineMesh;
class OBBCube;
class Camera;

/// @brief OBBのデバッグ描画
class OBBRenderer {
public:
    /// @brief コンストラクタ
    OBBRenderer();

    /// @brief デストラクタ
    ~OBBRenderer();

    /// @brief 初期化
    void Initialize();

    /// @brief 更新
    /// @param obb OBB
    void Update(const OBB& obb);

    /// @brief 描画
    /// @param camera カメラ
    void Draw(Camera& camera);

private:
    using LineMeshes = std::array<std::unique_ptr<LineMesh>, 12>;
    LineMeshes lines_;	// 線分メッシュ

    std::unique_ptr<OBBCube> cube_;	// 立方体メッシュ
};