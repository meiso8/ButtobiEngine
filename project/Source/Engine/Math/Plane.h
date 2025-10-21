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

#ifdef _DEBUG
/// @brief 編集
/// @param label ラベル
/// @param plane 平面
void EditPlane(const std::string &label, Plane &plane);
#endif // _DEBUG

class LineMesh;
class Camera;

/// @brief 平面のデバッグ描画
class PlaneRenderer {
public:
	/// @brief コンストラクタを追加しました・yoshida
	PlaneRenderer();

	/// @brief デフォルトデストラクタ
	~PlaneRenderer();

	/// @brief 初期化
	void Initialize();
	
	/// @brief 更新
	/// @param plane 平面
	void Update(const Plane &plane);
	
	/// @brief 描画
	/// @param camera カメラ
    void Draw(Camera &camera);
	
private:
	using LineMeshes = std::array<std::unique_ptr<LineMesh>, 4>;
	LineMeshes lines_;	// 線分メッシュ
};
