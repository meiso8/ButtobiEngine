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
	void Update();
	
	/// @brief 描画
	/// @param camera カメラ
	void Draw(Camera &camera);
	
	/// @brief AABBの設定
	/// @param aabb AABB
	void SetAABB(const AABB &aabb) { aabb_ = aabb; }
	
	/// @brief AABBの取得
	/// @return AABB
	AABB &GetAABB() { return aabb_; }

#ifdef _DEBUG
	/// @brief 編集
	/// @param label ラベル
	void Edit(const std::string &label);
#endif // _DEBUG

private:
	using LineMeshes = std::array<std::unique_ptr<LineMesh>, 12>;
	AABB aabb_;			// AABB
	LineMeshes lines_;	// 線分メッシュ
};