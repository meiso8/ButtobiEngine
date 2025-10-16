#pragma once
#include "Vector3.h"
#include <array>
#include <memory>
#include <string>

struct OBB {
	Vector3 center;		// 中心点
	Vector3 axis[3];	// 各軸の単位ベクトル
	Vector3 halfSizes;	// 各軸方向の半分の長さ
};

class LineMesh;
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
	void Update();

	/// @brief 描画
	/// @param camera カメラ
	void Draw(Camera &camera);

	/// @brief 各軸の単位ベクトルの設定
	/// @param rotate 回転角
	void SetAxis(const Vector3 &rotate);

	/// @brief 中心点の設定
	/// @param center 中心点
	void SetCenter(const Vector3 &center) { obb_.center = center; }

	/// @brief 各軸方向の半分の長さの設定
	/// @param halfSizes 各軸方向の半分の長さ
	void SetHalfSizes(const Vector3 &halfSizes) { obb_.halfSizes = halfSizes; }

	/// @brief OBBの取得
	/// @return OBB
	OBB &GetOBB() { return obb_; }

#ifdef _DEBUG
	/// @brief 編集
	/// @param label ラベル
	void Edit(const std::string &label);
#endif // _DEBUG

private:
	using LineMeshes = std::array<std::unique_ptr<LineMesh>, 12>;
	OBB obb_;			// OBB
	Vector3 rotate_;	// 回転角
	LineMeshes lines_;	// 線分メッシュ
};