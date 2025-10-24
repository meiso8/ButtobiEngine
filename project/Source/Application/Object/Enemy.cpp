#define NOMINMAX
#include "Enemy.h"
#include "AABB.h"
#include "Sphere.h"
#include "Player.h" // 追加
#include "Model.h"
#include "RigidBody.h"
#include "CollisionConfig.h"
#include "Collision.h"
#include <cmath>
#include <numbers>
#include <cassert>
#include"Sound.h"
#include"Particle/AppleCrashParticle.h"

#ifdef _DEBUG
#include "../externals/imgui/imgui.h"
#include"DebugUI.h"
#endif // _DEBUG

Enemy::Enemy() {
    model_ = std::make_unique<Model>();
    model_->Create(ModelManager::FRUIT_APPLE);
}

Enemy::~Enemy() = default;

void Enemy::Initialize(const Vector3& position) {

    color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
    worldTransform_.Initialize();
    worldTransform_.translate_ = position; // 初期位置をオリジンにしておく
    worldTransform_.rotate_.y = std::numbers::pi_v<float> *3.0f / 2.0f;

    walkTimer_ = 0.0f;

    // 剛体の生成
    rigidBody_ = std::make_unique<RigidBody>();
    rigidBody_->Initialize(1.0f, kRadius);

    isKicked_ = false;

#ifdef _DEBUG
    // AABBのデバッグ描画の生成と初期化
    isExistAABB_ = false;
    if (isExistAABB_) {
        aabbRenderer_ = std::make_unique<AABBRenderer>();
        aabbRenderer_->Initialize();
    }

    // 球のデバッグ描画の生成と初期化
    isExistSphere_ = false;
    if (isExistSphere_) {
        sphereRenderer_ = std::make_unique<SphereRenderer>();
        sphereRenderer_->Initialize();
    }
#endif // _DEBUG
}

void Enemy::Update() {

    // 移動
    rigidBody_->Update(1.0f / 60.0f);
    worldTransform_.rotate_ = rigidBody_->GetAngle();
    worldTransform_.translate_ += rigidBody_->GetVelocity() / 60.0f;

    if (isKicked_) {
        color_ = { 0.0f, 1.0f, 0.0f, 1.0f };
        //キックされた継続時間を足す
        if (kickDurationTimer_ < 600) {
            //10秒
            kickDurationTimer_++;
        } else {
            isKicked_ = false;
        }
    } else {
        //色を赤に戻す
        color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
    }

    // ==============================
    // 行列を定数バッファに転送
    // ==============================

    WorldTransformUpdate(worldTransform_);

#ifdef _DEBUG
    // AABBのデバッグ描画の更新
    if (isExistAABB_) {
        aabbRenderer_->Update(GetAABB());
    }

    // 球のデバッグ描画の更新
    if (isExistSphere_) {
        sphereRenderer_->Update(GetSphere());
    }
#endif // _DEBUG

}

void Enemy::Draw(Camera& camera) {
#ifdef _DEBUG

    // 球のデバッグ描画
    if (isExistSphere_) {
        sphereRenderer_->Draw(camera);
    }

    // AABBのデバッグ描画
    if (isExistAABB_) {
        aabbRenderer_->Draw(camera);
    }

#endif // _DEBUG

    // 3Dモデル描画前処理
    model_->PreDraw(kBlendModeNormal);
    model_->SetColor(color_);
    // 3Dモデルを描画
    model_->Draw(camera, worldTransform_.matWorld_, MaterialResource::LIGHTTYPE::HALF_L);
}

AABB Enemy::GetAABB() const {
    Vector3 worldPos = GetWorldPosition();
    AABB aabb = {
        .min = { worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f },
        .max = { worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f }
    };
    return aabb;
}

Sphere Enemy::GetSphere() const { return Sphere{ .center = GetWorldPosition(), .radius = kRadius }; }

Vector3 Enemy::GetWorldPosition() const { return { worldTransform_.matWorld_.m[3][0],worldTransform_.matWorld_.m[3][1],worldTransform_.matWorld_.m[3][2] }; }

void Enemy::OnCollision(Player* player) {

    color_ = { 1.0f, 1.0f, 0.0f, 1.0f };

    if (isKicked_) {
        return;
    }

    if (player->IsAttack()) {
        rigidBody_->ApplyForce(player->GetKickForce());
        isKicked_ = true;
        //キック継続時間の初期化
        kickDurationTimer_ = 0;
        //キック力を初期化
        player->InitKickForce();
        Sound::PlaySE(Sound::FRUIT_FALL);

    }

}

void Enemy::OnCollision(const Plane& plane) {
    constexpr float exz = 1.0f;								// XZ軸の反発係数
    constexpr float ey = 0.8f;								// Y軸の反発係数
    float distance = Distance(GetSphere().center, plane);	// 球の中心から平面までの距離
    float penetration = GetSphere().radius - distance;		// 球の半径と距離の差分を貫入量に設定

    // 貫入量が0以下なら衝突していないので処理を抜ける
    if (penetration <= 0.0f) {
        return;
    }

    Vector3 contactPoint = ClosestPoint(GetSphere().center, plane);			// 球の中心から最も近い点を取得
    float reflected = Dot(rigidBody_->GetVelocity() * 60.0f, plane.normal);	// 反射ベクトルの計算

    // 貫入していない場合は処理を抜ける
    if (reflected > 0.0f) {
        return;
    }

    float restitution = (std::fabs(plane.normal.y) > 0.707f) ? ey : exz;	// 反発係数の設定
    float j = -(1.0f + restitution) * reflected * rigidBody_->GetMass();	// 衝突インパルスの計算
    Vector3 impulse = plane.normal * j;										// 衝突インパルスベクトル
    rigidBody_->ApplyForce(impulse);										// 衝突インパルスをフルーツに加える
    rigidBody_->SetLeverArm(contactPoint - GetSphere().center);				// 重心から接触点までのベクトルを設定
    worldTransform_.translate_ += plane.normal * penetration;				// 貫入量分フルーツの位置を修正
}

void Enemy::OnCollision(const OBB& obb) {
    constexpr float exz = 1.0f;							// XZ軸の反発係数
    constexpr float ey = 0.8f;							// Y軸の反発係数
    float distance = Distance(GetSphere().center, obb);	// 球の中心からOBBまでの距離
    float penetration = GetSphere().radius - distance;  // 球の半径と距離の差分を貫入量に設定

    // 貫入量が0以下なら衝突していないので処理を抜ける
    if (penetration <= 0.0f) {
        return;
    }

    Vector3 contactPoint = ClosestPoint(GetSphere().center, obb);		// 球の中心から最も近い点を取得
    Vector3 normal = Normalize(GetSphere().center - contactPoint);		// 法線ベクトルの計算
    float reflected = Dot(rigidBody_->GetVelocity() * 60.0f, normal);	// 反射ベクトルの計算

    // 貫入していない場合は処理を抜ける
    if (reflected > 0.0f) {
        return;
    }

    float restitution = (std::fabs(normal.y) > 0.707f) ? ey : exz;			// 反発係数の設定
    float j = -(1.0f + restitution) * reflected * rigidBody_->GetMass();	// 衝突インパルスの計算
    Vector3 impulse = normal * j;											// 衝突インパルスベクトル
    rigidBody_->ApplyForce(impulse);										// 衝突インパルスをフルーツに加える
    rigidBody_->SetLeverArm(contactPoint - GetSphere().center);				// 重心から接触点までのベクトルを設定
    worldTransform_.translate_ += normal * penetration;						// 貫入量分フルーツの位置を修正
}

void Enemy::OnCollision() {

    isDead_ = true;

    if (crashParticle_ != nullptr) {
        crashParticle_->emitter_.transform.translate = GetWorldPosition();
        crashParticle_->EmitParticle(true, { 0.5f,0.5f,0.5f }, { 1.0f, 1.0f, 1.0f, 1.0f });
    }
}

Vector3 Enemy::GetVelocity() const { return rigidBody_->GetVelocity(); }

#ifdef _DEBUG
void Enemy::Edit(const std::string& label) {
    if (ImGui::TreeNode(label.c_str())) {
        ImGui::DragFloat3("translate", &worldTransform_.translate_.x, 0.01f, -std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
        ImGui::SliderAngle("rotateX", &worldTransform_.rotate_.x, -360.0f, 360.0f);
        ImGui::SliderAngle("rotateY", &worldTransform_.rotate_.y, -360.0f, 360.0f);
        ImGui::SliderAngle("rotateZ", &worldTransform_.rotate_.z, -360.0f, 360.0f);
        rigidBody_->Edit("RigidBody");
        ImGui::Checkbox("DrawAABB", &isExistAABB_);
        ImGui::Checkbox("DrawSphere", &isExistSphere_);
        ImGui::TreePop();
    }
}
#endif // _DEBUG