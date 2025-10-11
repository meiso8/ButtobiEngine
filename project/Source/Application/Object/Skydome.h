
#pragma once
#include"WorldTransform.h"

class Model;
class Camera;

/// @brief 天球
class Skydome {
public:
    /// @brief 初期化
    void Initialize(Camera* camera);
    /// @brief 更新
    void Update();
    /// @brief 描画
    void Draw();
    ~Skydome();
private:
    // ワールド変換データ
    WorldTransform worldTransform_;
    // モデル
    Model* model_ = nullptr;
    // カメラ
    Camera* camera_ = nullptr;
};