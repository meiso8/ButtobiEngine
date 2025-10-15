#pragma once
#include "Vector3.h"
#include <array>
#include <memory>
#include <string>
#include <numbers>

/// @brief 球
struct Sphere {
	Vector3 center;	// 中心座標
	float radius;	// 半径
};

class LineMesh;
class Camera;

/// @brief 球のデバッグ描画
class SphereRenderer {
public:
	/// @brief コンストラクタ
	SphereRenderer();

	/// @brief デストラクタ
	~SphereRenderer();

	/// @brief 初期化
	void Initialize();

	/// @brief 更新
	void Update();

	/// @brief 描画
	/// @param camera カメラ
	void Draw(Camera &camera);

	/// @brief 球の設定
	/// @param sphere 球
	void SetSphere(const Sphere &sphere) { sphere_ = sphere; }

	/// @brief 球の取得
	/// @return 球
	Sphere &GetSphere() { return sphere_; }

#ifdef _DEBUG
	/// @brief 編集
	/// @param label ラベル
	void Edit(const std::string &label);
#endif // _DEBUG

private:
	static inline constexpr uint32_t kSubdivision = 8;																// 分割数
	static inline constexpr float kLonEvery = std::numbers::pi_v<float> *2.0f / static_cast<float>(kSubdivision);	// 経度分割1つ分の角度
	static inline constexpr float kLatEvery = std::numbers::pi_v<float> / static_cast<float>(kSubdivision);			// 緯度分割1つ分の角度
	using LineMeshes = std::array<std::unique_ptr<LineMesh>, kSubdivision * kSubdivision * 2>;
	Sphere sphere_;		// 球
	LineMeshes lines_;	// 線分メッシュ
};