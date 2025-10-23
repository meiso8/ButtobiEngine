#define NOMINMAX // std::maxの置き換えが行われないため

#include "Player.h"
#include "AABB.h"

#include "Sphere.h"

#include "Collision.h"
#include "Enemy.h"
#include "Lerp.h"
#include "WorldTransform.h"
#include "MakeMatrix.h"
#include "Transform.h"
#include <algorithm>
#include <numbers>
#include<cmath>
#include"Input.h"
#include"ImGuiClass.h"
#include"Log.h"
#include"Model.h"
#include"CoordinateTransform.h"
#include"Sound.h"

Player::Player() {
    for (size_t i = 0; i < model_.size(); ++i) {
        model_[i] = new Model();
    }

    model_[Parts::kHead]->Create(ModelManager::HEAD);
    model_[Parts::kBody]->Create(ModelManager::BODY);
    model_[Parts::kLeftArm]->Create(ModelManager::LEFTARM);
    model_[Parts::kRightArm]->Create(ModelManager::RIGHTARM);
    model_[Parts::kLeftLeg]->Create(ModelManager::LEFTLEG);
    model_[Parts::kRightLeg]->Create(ModelManager::RIGHTLEG);

#ifdef _DEBUG
    // AABBのデバッグ描画の生成
    //aabbRenderer_ = std::make_unique<AABBRenderer>();


    // 球のデバッグ描画の生成
    sphereRenderer_ = std::make_unique<SphereRenderer>();
#endif // _DEBUG

}

Player::~Player() {
    for (size_t i = 0; i < model_.size(); ++i) {
        delete model_[i];
        model_[i] = nullptr; // 安全のためにnullptrにしておくといいよ！
    }

}

void Player::Initialize(Camera& camera, const Vector3& position) {

    camera_ = &camera;

    //アタックフェーズの初期化
    attackPhase_ = AttackPhase::kNone;

    // ワールド変換の初期化
    worldTransform_.Initialize();
    worldTransform_.translate_ = position;
    worldTransform_.rotate_.y = 0.0f;
    for (int i = 0; i < Parts::kNumParts; i++) {
        PartsWorldTransform_[i].Initialize();
        PartsWorldTransform_[i].scale_ = worldTransform_.scale_;
        PartsWorldTransform_[i].rotate_ = worldTransform_.rotate_;
        PartsWorldTransform_[i].translate_ = worldTransform_.translate_;
        // 合成結果
        PartsWorldTransform_[i].parent_ = &worldTransform_;

        DrawPartsWorldTransform_[i].Initialize();
    }

    defaultPartsOffset_[Parts::kHead] = { 0, 0.5f, 0 };
    defaultPartsOffset_[Parts::kBody] = { 0, -0.2f, 0 };
    defaultPartsOffset_[Parts::kLeftArm] = { -0.8f, 0.0f, 0 };
    defaultPartsOffset_[Parts::kRightArm] = { 0.8f, 0.0f, 0 };
    defaultPartsOffset_[Parts::kLeftLeg] = { -0.2f, -1.0f, 0 };
    defaultPartsOffset_[Parts::kRightLeg] = { 0.2f, -1.0f, 0 };

#pragma region Head

    targetPartsScale_[AttackPhase::kNone][Parts::kHead] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kNone][Parts::kHead] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kNone][Parts::kHead] = defaultPartsOffset_[Parts::kHead];

    targetPartsScale_[AttackPhase::kCharge][Parts::kHead] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kCharge][Parts::kHead] = { 1.0f, 0.0f, 0.1f };
    targetPartsTranslate_[AttackPhase::kCharge][Parts::kHead] = defaultPartsOffset_[Parts::kHead] + Vector3{ 0.0f, -0.4f, 0.8f };

    targetPartsScale_[AttackPhase::kFire][Parts::kHead] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kFire][Parts::kHead] = { 0.0f, 0.0f, 0.1f };
    targetPartsTranslate_[AttackPhase::kFire][Parts::kHead] = defaultPartsOffset_[Parts::kHead];

    targetPartsScale_[AttackPhase::kEnd][Parts::kHead] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kEnd][Parts::kHead] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kEnd][Parts::kHead] = defaultPartsOffset_[Parts::kHead];

#pragma endregion

#pragma region Body

    targetPartsScale_[AttackPhase::kNone][Parts::kBody] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kNone][Parts::kBody] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kNone][Parts::kBody] = defaultPartsOffset_[Parts::kBody];

    targetPartsScale_[AttackPhase::kCharge][Parts::kBody] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kCharge][Parts::kBody] = { 1.5f, 0.0f, 0.2f };
    targetPartsTranslate_[AttackPhase::kCharge][Parts::kBody] = defaultPartsOffset_[Parts::kBody] + Vector3{ 0.0f, 0.0f, 0.2f };

    targetPartsScale_[AttackPhase::kFire][Parts::kBody] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kFire][Parts::kBody] = { -0.3f, 0.2f, 0.0f };
    targetPartsTranslate_[AttackPhase::kFire][Parts::kBody] = defaultPartsOffset_[Parts::kBody];

    targetPartsScale_[AttackPhase::kEnd][Parts::kBody] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kEnd][Parts::kBody] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kEnd][Parts::kBody] = defaultPartsOffset_[Parts::kBody];

#pragma endregion

#pragma region LeftArm

    targetPartsScale_[AttackPhase::kNone][Parts::kLeftArm] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kNone][Parts::kLeftArm] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kNone][Parts::kLeftArm] = defaultPartsOffset_[Parts::kLeftArm];

    targetPartsScale_[AttackPhase::kCharge][Parts::kLeftArm] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kCharge][Parts::kLeftArm] = { 1.5f, 0.0f, -0.3f };
    targetPartsTranslate_[AttackPhase::kCharge][Parts::kLeftArm] = defaultPartsOffset_[Parts::kLeftArm] + Vector3{ 0.0f, -0.18f, 0.3f };

    targetPartsScale_[AttackPhase::kFire][Parts::kLeftArm] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kFire][Parts::kLeftArm] = { 0.0f, 0.0f, 0.2f };
    targetPartsTranslate_[AttackPhase::kFire][Parts::kLeftArm] = defaultPartsOffset_[Parts::kLeftArm] + Vector3{ 0.2f, -0.2f, -0.2f };

    targetPartsScale_[AttackPhase::kEnd][Parts::kLeftArm] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kEnd][Parts::kLeftArm] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kEnd][Parts::kLeftArm] = defaultPartsOffset_[Parts::kLeftArm];

#pragma endregion

#pragma region RightArm

    targetPartsScale_[AttackPhase::kNone][Parts::kRightArm] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kNone][Parts::kRightArm] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kNone][Parts::kRightArm] = defaultPartsOffset_[Parts::kRightArm];

    targetPartsScale_[AttackPhase::kCharge][Parts::kRightArm] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kCharge][Parts::kRightArm] = { 1.5f, 0.0f, -0.2f };
    targetPartsTranslate_[AttackPhase::kCharge][Parts::kRightArm] = defaultPartsOffset_[Parts::kRightArm] + Vector3{ 0.0f, -0.28f, 0.3f };

    targetPartsScale_[AttackPhase::kFire][Parts::kRightArm] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kFire][Parts::kRightArm] = { 0.0f, 0.3f, 0.0f };
    targetPartsTranslate_[AttackPhase::kFire][Parts::kRightArm] = defaultPartsOffset_[Parts::kRightArm] + Vector3{ 0.0f, 0.0f, -0.2f };

    targetPartsScale_[AttackPhase::kEnd][Parts::kRightArm] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kEnd][Parts::kRightArm] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kEnd][Parts::kRightArm] = defaultPartsOffset_[Parts::kRightArm];

#pragma endregion

#pragma region LeftLeg

    targetPartsScale_[AttackPhase::kNone][Parts::kLeftLeg] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kNone][Parts::kLeftLeg] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kNone][Parts::kLeftLeg] = defaultPartsOffset_[Parts::kLeftLeg];

    targetPartsScale_[AttackPhase::kCharge][Parts::kLeftLeg] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kCharge][Parts::kLeftLeg] = { 0.0f, 0.0f, 0.1f };
    targetPartsTranslate_[AttackPhase::kCharge][Parts::kLeftLeg] = defaultPartsOffset_[Parts::kLeftLeg];

    targetPartsScale_[AttackPhase::kFire][Parts::kLeftLeg] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kFire][Parts::kLeftLeg] = { -0.2f, 0.0f, 0.1f };
    targetPartsTranslate_[AttackPhase::kFire][Parts::kLeftLeg] = defaultPartsOffset_[Parts::kLeftLeg] + Vector3{ 0.1f, 0.0f, 0.0f };

    targetPartsScale_[AttackPhase::kEnd][Parts::kLeftLeg] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kEnd][Parts::kLeftLeg] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kEnd][Parts::kLeftLeg] = defaultPartsOffset_[Parts::kLeftLeg];

#pragma endregion

#pragma region RightLeg

    targetPartsScale_[AttackPhase::kNone][Parts::kRightLeg] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kNone][Parts::kRightLeg] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kNone][Parts::kRightLeg] = defaultPartsOffset_[Parts::kRightLeg];

    targetPartsScale_[AttackPhase::kCharge][Parts::kRightLeg] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kCharge][Parts::kRightLeg] = { 2.5f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kCharge][Parts::kRightLeg] = defaultPartsOffset_[Parts::kRightLeg] + Vector3{ 0.0f, 1.2f, -0.6f };

    targetPartsScale_[AttackPhase::kFire][Parts::kRightLeg] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kFire][Parts::kRightLeg] = { -2.0f, -0.6f, -0.2f };
    targetPartsTranslate_[AttackPhase::kFire][Parts::kRightLeg] = defaultPartsOffset_[Parts::kRightLeg] + Vector3{ -0.2f, 0.7f, 0.5f };

    targetPartsScale_[AttackPhase::kEnd][Parts::kRightLeg] = { 1.0f, 1.0f, 1.0f };
    targetPartsRotate_[AttackPhase::kEnd][Parts::kRightLeg] = { 0.0f, 0.0f, 0.0f };
    targetPartsTranslate_[AttackPhase::kEnd][Parts::kRightLeg] = defaultPartsOffset_[Parts::kRightLeg];

#pragma endregion

    objectColor_ = { 1.0f,1.0f,1.0f,1.0f };

    chargeTimer_ = 100.0f;

#ifdef _DEBUG
    // AABBのデバッグ描画の初期化
    //aabbRenderer_->Initialize();

    // 球のデバッグ描画の初期化
    sphereRenderer_->Initialize();
#endif // _DEBUG

}

void Player::InputMove() {
    // 左右キーは見た目だけを回転させる（移動には影響しない）
    if (Input::IsPushKey(DIK_RIGHT) || Input::IsPushKey(DIK_D)) {
        worldTransform_.rotate_.y += 0.03f;
    } else if (Input::IsPushKey(DIK_LEFT) || Input::IsPushKey(DIK_A)) {
        worldTransform_.rotate_.y -= 0.03f;
    }

    // 前後移動（常にカメラ基準）
    if (Input::IsPushKey(DIK_UP) || Input::IsPushKey(DIK_W) ||
        Input::IsPushKey(DIK_DOWN) || Input::IsPushKey(DIK_S)) {

        // カメラの向きを基準に前方ベクトルを計算
        Vector3 forward = { std::sin(worldTransform_.rotate_.y), 0.0f, std::cos(worldTransform_.rotate_.y) };
        forward = Normalize(forward);

        if (Input::IsPushKey(DIK_UP) || Input::IsPushKey(DIK_W)) {
            worldTransform_.translate_ += forward * kMoveAmount;
        }
        if (Input::IsPushKey(DIK_DOWN) || Input::IsPushKey(DIK_S)) {
            worldTransform_.translate_ -= forward * kMoveAmount;
        }
    }
}

inline bool NearlyEqual(const Vector3& a, const Vector3& b, float epsilon = 0.001f) {
    return fabs(a.x - b.x) < epsilon && fabs(a.y - b.y) < epsilon && fabs(a.z - b.z) < epsilon;
}

void Player::InputAttack() {

    if (Input::IsTriggerKey(DIK_1)) {
        attackPhase_ = kNone;
        ResetAttack();
    }
    if (Input::IsTriggerKey(DIK_2)) {
        attackPhase_ = kCharge;
    }
    if (Input::IsTriggerKey(DIK_3)) {
        attackPhase_ = kFire;
    }
    if (Input::IsTriggerKey(DIK_4)) {
        attackPhase_ = kEnd;
    }

    bool allMatched = true;
    switch (attackPhase_) {
    case Player::kNone:


        if (Input::IsPushKey(DIK_SPACE)) {

            attackPhase_ = Player::kCharge;
            Sound::PlayLoopSE(Sound::CHARGE, 0.0f);
        }


        break;
    case Player::kCharge:

        if (!Input::IsPushKey(DIK_SPACE)) {
            attackPhase_ = Player::kFire;
            Sound::Stop(Sound::CHARGE);
            Sound::PlaySE(Sound::ATTACK, 0.0f);
            isAttack_ = true;
        }

        //チャージを加算する
        if (chargeTimer_ < kMaxChargeTime) {
            chargeTimer_ += 100;
        }

        break;
    case Player::kFire:

        isAttack_ = true;

        for (int i = 0; i < Parts::kNumParts; i++) {
            if (!(NearlyEqual(PartsWorldTransform_[i].scale_, targetPartsScale_[AttackPhase::kFire][i]) && NearlyEqual(PartsWorldTransform_[i].rotate_, targetPartsRotate_[AttackPhase::kFire][i]) &&
                NearlyEqual(PartsWorldTransform_[i].translate_, targetPartsTranslate_[AttackPhase::kFire][i]))) {
                allMatched = false;
                break;
            }
        }
        if (allMatched) {
            attackPhase_ = Player::kEnd;
        }


        break;
    case Player::kEnd:

        //isAttack_ = false;


        if (isEndAninationEnd_) {
            isEndAninationEnd_ = false;
            endAninationTimer_ = 0;
            attackPhase_ = Player::kNone;
            ResetAttack();
        } else {
            endAninationTimer_++;
            if (endAninationTimer_ >= endAninationTimerMax_ * 60) {
                endAninationTimer_ = 0;
                isEndAninationEnd_ = true;
            }
        }

        break;
    default:
        break;
    }

}


void Player::AttackAnimation() {
    for (int i = 0; i < Parts::kNumParts; i++) {
        PartsWorldTransform_[i].scale_ = Lerp(PartsWorldTransform_[i].scale_, targetPartsScale_[attackPhase_][i], kInterVal_);
        PartsWorldTransform_[i].rotate_ = Lerp(PartsWorldTransform_[i].rotate_, targetPartsRotate_[attackPhase_][i], kInterVal_);
        PartsWorldTransform_[i].translate_ = Lerp(PartsWorldTransform_[i].translate_, targetPartsTranslate_[attackPhase_][i], kInterVal_);

        // ローカル行列を作成（補間済み）
        Matrix4x4 localMat = MakeAffineMatrix(PartsWorldTransform_[i].scale_, PartsWorldTransform_[i].rotate_, PartsWorldTransform_[i].translate_ + defaultPartsOffset_[i]);

        // 親（プレイヤー本体）のワールド行列
        Matrix4x4 parentMat = MakeAffineMatrix(worldTransform_.scale_, worldTransform_.rotate_, worldTransform_.translate_);


        WorldTransformUpdate(PartsWorldTransform_[i]);
    }
    WorldTransformUpdate(worldTransform_);

}

Vector3 Player::GetForward() const {
    return Normalize({ worldTransform_.matWorld_.m[2][0], worldTransform_.matWorld_.m[2][1], worldTransform_.matWorld_.m[2][2] });
}

Vector3 Player::GetWorldPosition() const { return worldTransform_.GetWorldPosition(); }

void Player::Update() {
    // ==============================
    // 1.移動入力
    // ==============================

    InputMove();

    InputAttack();

    // ==============================
    // 2.移動処理
    // ==============================

    Vector3 acceleration = { 0.0f, -kGravityAcceleration, 0.0f };
    velocity_ += acceleration / 60.0f;
    worldTransform_.translate_ += velocity_ / 60.0f;

    // ==============================
    // 3.無敵処理
    // ==============================

    if (isInvincible_) {
        invincibleTimer_++;
        if (invincibleTimer_ >= invincibleTimerMax_ * 60) {
            invincibleTimer_ = 0;
            isInvincible_ = false;
            // 色戻し(仮処理)
            objectColor_ = { 1.0f,1.0f,1.0f,1.0f };
        }
    }

    // ==============================
    // 4.ワールド変換の更新
    // ==============================

    WorldTransformUpdate(worldTransform_);

    // ==============================
    // 5.攻撃アニメーション処理
    // ==============================

    AttackAnimation();

    // ==============================
    // 6.デバッグ描画の更新
    // ==============================

#ifdef _DEBUG
    // AABBのデバッグ描画の更新
    //aabbRenderer_->Update(GetKickAreaAABB());


    // 球のデバッグ描画の更新
    sphereRenderer_->Update(GetSphere());
#endif // _DEBUG
}

void Player::Draw(Camera& camera) {
#ifdef _DEBUG
    // AABBのデバッグ描画の描画
    //aabbRenderer_->Draw(camera);

    // 球のデバッグ描画の描画
    sphereRenderer_->Draw(camera);

#endif // _DEBUG

    // 3Dモデル描画前処理
    model_[0]->PreDraw(BlendMode::kBlendModeNormal);

    // 3Dモデルを描画
    for (int i = 0; i < Parts::kNumParts; i++) {
        //ここに追加しました。
        model_[i]->SetColor(objectColor_);

        model_[i]->Draw(camera, PartsWorldTransform_[i].matWorld_, MaterialResource::LIGHTTYPE::HALF_L);
    }
}



Sphere Player::GetSphere() const { return Sphere{ .center = GetWorldPosition(), .radius = kRadius }; }

// 衝突応答
void Player::OnCollision(const Enemy* enemy) {
    if (isAttack_) {
        ResetAttack();
    } else {
        if (IsCollision(GetSphere(), enemy->GetSphere())) {

            if (!isInvincible_) {
                // 色変え(仮処理)
                objectColor_ = { 0.0f,0.0f,0.0f,1.0f };

                life_--;
                isInvincible_ = true;
                Sound::PlaySE(Sound::PLAYER_HIT);
            }

        }
    }
};


void Player::OnCollision(const Plane& plane) {
    float penetration = PenetrationDepth(GetSphere(), plane);	// 貫入量

    // 貫入量が0以下なら衝突していないので処理を抜ける
    if (penetration <= 0.0f) {
        return;
    }

    velocity_ -= Dot(velocity_, plane.normal) * plane.normal;	// 法線方向の速度成分を打ち消す
    velocity_.x = 0.0f; // 水平方向の速度をゼロにする
    velocity_.z = 0.0f;	// 水平方向の速度をゼロにする
    worldTransform_.translate_ += plane.normal * penetration;	// 貫入量分だけ位置を修正
}

void Player::OnCollision(const OBB& obb) {
    // 貫入量
    float penetration = PenetrationDepth(GetSphere(), obb);

    // 貫入量が0以下なら衝突していないので処理を抜ける
    if (penetration <= 0.0f) {
        return;
    }

    Vector3 contactPoint = ClosestPoint(GetSphere().center, obb);	// 球の中心から最も近い点を取得
    Vector3 normal = Normalize(GetSphere().center - contactPoint);	// 法線ベクトルの計算

    velocity_ -= Dot(velocity_, normal) * normal;		// 法線方向の速度成分を打ち消す
    worldTransform_.translate_ += normal * penetration;	// 貫入量分だけ位置を修正
}

bool Player::IsAttack() const { return isAttack_; }

bool Player::IsCharge() const {
    if (attackPhase_ == AttackPhase::kCharge) {
        return true;
    }

    return false;
}

Vector3 Player::GetAttackArea()
{
    return GetWorldPosition() + GetForward() * 5.0f;
}

void Player::Debug() {
    ImGui::Begin("Parts");

    ImGui::Text("Head");
    ImGui::DragFloat3("HeadScale", &targetPartsScale_[attackPhase_][Parts::kHead].x, 0.1f);
    ImGui::DragFloat3("HeadRotate", &targetPartsRotate_[attackPhase_][Parts::kHead].x, 0.1f);
    ImGui::DragFloat3("HeadTranslate", &targetPartsTranslate_[attackPhase_][Parts::kHead].x, 0.1f);
    if (ImGui::Button("HeadReset")) {
        targetPartsScale_[attackPhase_][Parts::kHead] = { 1.0f, 1.0f, 1.0f };
        targetPartsRotate_[attackPhase_][Parts::kHead] = { 0.0f, 0.0f, 0.0f };
        targetPartsTranslate_[attackPhase_][Parts::kHead] = { 0.0f, 0.0f, 0.0f };
    }
    ImGui::Text("Body");
    ImGui::DragFloat3("BodyScale", &targetPartsScale_[attackPhase_][Parts::kBody].x, 0.1f);
    ImGui::DragFloat3("BodyRotate", &targetPartsRotate_[attackPhase_][Parts::kBody].x, 0.1f);
    ImGui::DragFloat3("BodyTranslate", &PartsWorldTransform_[Parts::kBody].translate_.x, 0.1f);
    if (ImGui::Button("BodyReset")) {
        targetPartsScale_[attackPhase_][Parts::kBody] = { 1.0f, 1.0f, 1.0f };
        targetPartsRotate_[attackPhase_][Parts::kBody] = { 0.0f, 0.0f, 0.0f };
        targetPartsTranslate_[attackPhase_][Parts::kBody] = { 0.0f, 0.0f, 0.0f };
    }

    ImGui::Separator();
    ImGui::Text("LeftArm");
    ImGui::DragFloat3("LeftArmScale", &targetPartsScale_[attackPhase_][Parts::kLeftArm].x, 0.1f);
    ImGui::DragFloat3("LeftArmRotate", &targetPartsRotate_[attackPhase_][Parts::kLeftArm].x, 0.1f);
    ImGui::DragFloat3("LeftArmTranslate", &targetPartsTranslate_[attackPhase_][Parts::kLeftArm].x, 0.1f);

    if (ImGui::Button("LeftArmReset")) {
        targetPartsScale_[attackPhase_][Parts::kLeftArm] = { 1.0f, 1.0f, 1.0f };
        targetPartsRotate_[attackPhase_][Parts::kLeftArm] = { 0.0f, 0.0f, 0.0f };
        targetPartsTranslate_[attackPhase_][Parts::kLeftArm] = { 0.0f, 0.0f, 0.0f };
    }

    ImGui::Text("RightArm");
    ImGui::DragFloat3("RightArmScale", &targetPartsScale_[attackPhase_][Parts::kRightArm].x, 0.1f);
    ImGui::DragFloat3("RightArmRotate", &targetPartsRotate_[attackPhase_][Parts::kRightArm].x, 0.1f);
    ImGui::DragFloat3("RightArmTranslate", &targetPartsTranslate_[attackPhase_][Parts::kRightArm].x, 0.1f);

    if (ImGui::Button("RightArmReset")) {
        targetPartsScale_[attackPhase_][Parts::kRightArm] = { 1.0f, 1.0f, 1.0f };
        targetPartsRotate_[attackPhase_][Parts::kRightArm] = { 0.0f, 0.0f, 0.0f };
        targetPartsTranslate_[attackPhase_][Parts::kRightArm] = { 0.0f, 0.0f, 0.0f };
    }

    ImGui::Separator();
    ImGui::Text("LeftLeg");
    ImGui::DragFloat3("LeftLegScale", &targetPartsScale_[attackPhase_][Parts::kLeftLeg].x, 0.1f);
    ImGui::DragFloat3("LeftLegRotate", &targetPartsRotate_[attackPhase_][Parts::kLeftLeg].x, 0.1f);
    ImGui::DragFloat3("LeftLegTranslate", &targetPartsTranslate_[attackPhase_][Parts::kLeftLeg].x, 0.1f);
    if (ImGui::Button("LeftLegReset")) {
        targetPartsScale_[attackPhase_][Parts::kLeftLeg] = { 1.0f, 1.0f, 1.0f };
        targetPartsRotate_[attackPhase_][Parts::kLeftLeg] = { 0.0f, 0.0f, 0.0f };
        targetPartsTranslate_[attackPhase_][Parts::kLeftLeg] = { 0.0f, 0.0f, 0.0f };
    }

    ImGui::Text("RightLeg");
    ImGui::DragFloat3("RightLegScale", &targetPartsScale_[attackPhase_][Parts::kRightLeg].x, 0.1f);
    ImGui::DragFloat3("RightLegRotate", &targetPartsRotate_[attackPhase_][Parts::kRightLeg].x, 0.1f);
    ImGui::DragFloat3("RightLegTranslate", &targetPartsTranslate_[attackPhase_][Parts::kRightLeg].x, 0.1f);
    if (ImGui::Button("RightLegReset")) {
        targetPartsScale_[attackPhase_][Parts::kRightLeg] = { 1.0f, 1.0f, 1.0f };
        targetPartsRotate_[attackPhase_][Parts::kRightLeg] = { 0.0f, 0.0f, 0.0f };
        targetPartsTranslate_[attackPhase_][Parts::kRightLeg] = { 0.0f, 0.0f, 0.0f };
    }
    ImGui::End();

    ImGui::Begin("Player");
    ImGui::DragFloat3("scale", &worldTransform_.scale_.x);
    ImGui::DragFloat3("rotate_", &worldTransform_.rotate_.x);
    ImGui::DragFloat3("translate_", &worldTransform_.translate_.x);
    ImGui::DragFloat3("velocity", &velocity_.x);

    ImGui::Separator();
    Vector3 worldPosition = GetWorldPosition();
    ImGui::DragFloat3("WorldPosition", &worldPosition.x);
    Vector3 forward = GetForward();
    ImGui::DragFloat3("Forward", &forward.x);
    ImGui::Separator();
    ImGui::DragFloat3("kickForce", &kickForce_.x);
    ImGui::Checkbox("isAttack", &isAttack_);
    ImGui::DragFloat("chargeTimer_", &chargeTimer_);



    switch (attackPhase_) {
    case Player::kNone:
        ImGui::Text("kNone");
        break;
    case Player::kCharge:
        ImGui::Text("kCharge");
        break;
    case Player::kFire:
        ImGui::Text("kFire");
        break;
    case Player::kEnd:
        ImGui::Text("kEnd");
        break;
    default:
        break;
    }

    //ImGui::Text("isWallHit : %d ", collisionMapInfo.isWallHit);
    //ImGui::DragFloat3("moveVol", &collisionMapInfo.moveVol.x);

    ImGui::End();
}

void Player::ResetAttack() {
    isAttack_ = false;
    kickForce_ = GetForward() * chargeTimer_;
    chargeTimer_ = 100.0f;
}
