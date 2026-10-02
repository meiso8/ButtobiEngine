#pragma once

#include"WorldTransform.h"
#include"AnimationObject3d.h"
#include"CharacterState.h"
#include"AABB.h"
#include"Collider.h"
#include"EyePosition.h"

class Model;
class Camera;
enum LightMode;
class CircleMesh;
class CubeMesh;
class Sprite;


class Player :public Collider
{
public:

    /// @brief コンストラクタ
    Player();
    /// @brief デストラクタ
    ~Player();
    /// @brief 初期化処理
    /// @param pos 位置を設定する
    void Init(const Vector3& pos);
    /// @brief 描画処理
    void Draw();
    /// @brief 更新
    void Update();
    /// @brief デバック表示
    void Debug();

    /// @brief 
    /// @return 
    const Vector3& GetEyeForward();
    const Vector3& GetBodyForward();
    /// @brief 目の位置の行列を得る
    /// @return 
    const Matrix4x4& GetEyeMatrix() {
        return eyePosition_->GetWorldMatrix();
    };
    /// @brief 目の位置のトランスフォームを得る
    /// @return 
    const WorldTransform& GetEyeWorldTransform() {
        return eyePosition_->GetWorldTransform();
    }
    /// @brief 目の位置のワールド座標を得る
    /// @return 
    const Vector3& GetEyeWorldPosition() const{ return worldEyePos_; };
    /// @brief 体のトランスフォームを得る
    /// @return 
    const WorldTransform& GetBodyWorldTransform() {
        return aniObject_->GetWorldTransform();
    }
    /// @brief 手の位置のマトリックスポインタを得る
    /// @return 手の位置
    const Matrix4x4* GetHandMatrixPtr() { return &handMatrix_; }
    /// @brief HPの構造体を得る
    /// @return HP構造体
    const HPs* GetHpsPtr() { return &characterState_.hps; }

    /// @brief 体の回転をセットする
    /// @param rotate 回転
    void SetBodyRotate(const Vector3& rotate) { aniObject_->SetRotate(rotate); }
    /// @brief 体のスケールをセットする
    /// @param scale セット
    void SetBodyScale(const Vector3& scale) { aniObject_->SetScale(scale); }

    /// @brief 衝突時判定
    /// @param collider 
    void OnCollision(Collider* collider)override;
    /// @brief 敵に衝突したときの処理
    /// @param hitPoint 
    void OnCollisionEnemy(const int hitPoint = 10);


    /// @brief　死亡判定を得る 
    /// @return 死亡判定
    const bool IsDead() { return characterState_.isDead; }
    /// @brief ズームタイマーを得る
    /// @return ズームタイマー
    const float& GetZoomTimer() { return zoomTimer_; }


    /// @brief ジャンプ可能かを設定する
    /// @param canJump ジャンプ可能か
    void SetCanJump(const bool canJump) { canJump_ = canJump; };
    /// @brief サーモグラフィー
    void Thermography();

private:
    void Move();
    void Jump();
    void Zoom();

    void MouseLook();
private:

    //目の位置
    std::unique_ptr<EyePosition>eyePosition_ = nullptr;
    Vector3 worldEyePos_{};

#pragma region//カメラ情報
    //カメラ速度
    float cameraSpeed_ = 1.0f;
    //カメラ回転Y
    float cameraRotateY_ = 0.0f;
    //カメラ回転X
    float cameraRotateX_ = 0.0f;
    //ズームタイマー
    float zoomTimer_ = 1.0f;
    //ズームフラグ
    bool isZoom_ = false;
    //ズーム開始タイマー
    float zoomStartTimer_ = 0.0f;
#pragma endregion

    //キャラクター状態
    CharacterState characterState_{};
    //無敵フラグ
    bool isInvincible_ = false;
    //ヒットタイマー
    float hitTimer_ = 0.0f;
    //床との衝突
    bool isFloorHit_ = false;

#pragma region//ジャンプ
    //ジャンプ可能かどうか
    bool canJump_ = false;
    //ジャンプフラグ
    bool isJump_ = false;
    //ジャンプスピード
    const float kJumpSpeed_ = 0.3125;
#pragma endregion

#pragma region//サーモグラフィー
    //サーモグラフィー有効フラグ
    bool isThermography_ = false;
    //サーモグラフィー終了取得
    bool isThermographyEnd_ = false;
    //サーモグラフィー度
    float thermography_ = 0.0f;
#pragma endregion

#pragma region//物理情報
    //速度
    Vector3 velocity_ = { 0.0f };
    //速度倍率を格納
    float speed_ = 0.0f;
#pragma endregion

    //音声が鳴るタイマー
    float soundTimer_ = 0.0f;


#pragma region//jointの行列を格納
    /// @brief 頭
    Matrix4x4 headMatrix_{};
    /// @brief 手
    Matrix4x4 handMatrix_{};
#pragma endregion

    //アニメーション付きオブジェクト
    std::unique_ptr<AnimationObject3d> aniObject_ = nullptr;
};

