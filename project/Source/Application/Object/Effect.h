#pragma once
#include"WorldTransform.h"
#include"Vector4.h"
class Model;
class Camera;

/// @brief 天球
class Effect {
public:
    Effect(const WorldTransform& parent);
    /// @brief 初期化
    void Initialize();
    /// @brief 更新
    void Update(const bool& isCharge,const Vector4& color);
    /// @brief 描画
    void Draw(Camera& camera);
    ~Effect();
private:
    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル
    Model* model_ = nullptr;
};