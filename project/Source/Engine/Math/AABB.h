#pragma once
#include"Vector3.h"
#include <array>
#include <memory>
#include <string>

//Axis Aligned Bounding Box
struct AABB {
    Vector3 min;
    Vector3 max;
};

#ifdef _DEBUG
/// @brief AABBの編集
/// @param label ラベル
/// @param aabb AABB
void EditAABB(const std::string &label, AABB &aabb);
#endif // _DEBUG

class LineMesh;
class Camera;

/// @brief AABBのデバッグ描画
class AABBRenderer {
public:
	/// @brief コンストラクタ
	AABBRenderer();
	
	/// @brief デストラクタ
	~AABBRenderer();
	
	/// @brief 初期化
	void Initialize();
	
	/// @brief 更新
	/// @param aabb AABB
	void Update(const AABB &aabb);
	
	/// @brief 描画
	/// @param camera カメラ
	void Draw(Camera &camera);
	
private:
	using LineMeshes = std::array<std::unique_ptr<LineMesh>, 12>;
	LineMeshes lines_;	// 線分メッシュ
};