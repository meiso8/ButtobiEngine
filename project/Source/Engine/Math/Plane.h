#pragma once
#include"Vector3.h"
#include <array>
#include <memory>
#include <string>

//無限遠平面
struct Plane {
    Vector3 normal;//法線
    float distance;//距離
};

class LineMesh;
class Camera;

/// @brief 平面のデバッグ描画
class PlaneRenderer {
public:
	/// @brief 初期化
	void Initialize();
	
	/// @brief 更新
	void Update();
	
	/// @brief 描画
	/// @param camera カメラ
    void Draw(Camera &camera);
	
	/// @brief 平面の設定
	/// @param plane 平面
	void SetPlane(const Plane &plane) { plane_ = plane; }
	
	/// @brief 平面の取得
	/// @return 平面
	Plane &GetPlane() { return plane_; }

#ifdef _DEBUG
	/// @brief 編集
	/// @param label ラベル
	void Edit(const std::string &label);
#endif // _DEBUG

private:
	using LineMeshes = std::array<std::unique_ptr<LineMesh>, 4>;
	Plane plane_;		// 平面
	LineMeshes lines_;	// 線分メッシュ
};
