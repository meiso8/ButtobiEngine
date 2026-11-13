#include "Enemy.h"
#include"Model.h"
#include"ModelManager.h"
#include"Camera.h"
#include"Input.h"

#include"JsonFile.h"
#include"CollisionConfig.h"
#include"SphericalCoordinate.h"
#include"Collision.h"
#include"Easing.h"
#include"DebugUI.h"
#include"TransformAni/TransformAni.h"
#include"TimeManager.h"
#include"Math/Matrix/MakeMatrix.h"

#include"IdleEnemy/IdleEnemy.h"
#include"ExitEnemy/ExitEnemy.h"
#include"RoundEnemy/RoundEnemy.h"
#include"ShotEnemy/ShotEnemy.h"
#include"BeamEnemy/BeamEnemy.h"
#include"AppearEnemy/AppearEnemy.h"
#include"AlphaWalkEnemy/AlphaWalkEnemy.h"

namespace {
    constexpr float kScale_ = 5.0f;

    constexpr float kColliderRad_ = 0.5f;
    constexpr float kWalkFootstepInterval_ = 0.4f;
    float soundTimer_ = 0.0f;
    bool isFootPreCollided_ = false;
}

Enemy::Enemy()
{

    model_ = ModelManager::GetModel("medjed.gltf");

    bodyPos_ = std::make_unique<AnimationObject3d>();
    bodyPos_->SetModelAndLoadAnimation(model_);
    bodyPos_->Create();
    bodyPos_->SetMeshAndMaterial(model_);

    bodyPos_->SetTemperature(1.0f);
    float halfScale = kScale_ * 0.25f;
    Init();
    SetAABB({ { -halfScale - halfScale * 1.5f ,-halfScale }, { halfScale ,halfScale * 1.5f ,halfScale } });
    SetWorldMatrix(bodyPos_->GetWorldTransform());
    SetCenter({ 0.0f,0.0f, 0.0f });

    SetCollisionAttribute(CollisionTag::GetTag("Enemy"));
    // 敵は「プレイヤー」と「プレイヤーの弾」と衝突したい
    SetCollisionMask(
        CollisionTag::GetTag("Player")
        | CollisionTag::GetTag("PlayerBulletCold")
        | CollisionTag::GetTag("PlayerBulletHot")
    );

    colliders_["EnemyFoot_L"].collider_ = std::make_unique<Collider>();
    colliders_["EnemyFoot_R"].collider_ = std::make_unique<Collider>();
    colliders_["EnemyHead"].collider_ = std::make_unique<Collider>();

    eyeMats_["eye_L"] = MakeIdentity4x4();
    eyeMats_["eye_R"] = MakeIdentity4x4();

    for (auto& [name, group] : colliders_) {
        group.matrix_ = MakeIdentity4x4();
        group.collider_->SetRadius(kColliderRad_);

        group.collider_->SetCollisionAttribute(CollisionTag::GetTag("Enemy"));
        // 足とfloorを判定する

        if (name == "EnemyHead") {
            //目だったら
            group.collider_->SetCollisionMask(CollisionTag::GetTag("PlayerBulletCold") | CollisionTag::GetTag("PlayerBulletCold"));
            /*     group.collider_->SetCenter({ 0.0f,0.0f,0.125f });*/
        } else {
            group.collider_->SetCollisionMask(CollisionTag::GetTag("Floor"));
            group.collider_->SetCenter({ 0.0f,0.125f,0.0f });
        }

        group.collider_->SetWorldMatrix(group.matrix_);
    }

    states_[IEnemy::STATE::APPEAR] = std::make_unique<AppearEnemy>();
    states_[IEnemy::STATE::ROUND] = std::make_unique<RoundEnemy>();
    states_[IEnemy::STATE::SHOT] = std::make_unique<ShotEnemy>();
    states_[IEnemy::STATE::ALPHA_WALK] = std::make_unique<AlphaWalkEnemy>();
    states_[IEnemy::STATE::BEAM] = std::make_unique<BeamEnemy>();
    states_[IEnemy::STATE::IDLE] = std::make_unique<IdleEnemy>();
    states_[IEnemy::STATE::EXIT] = std::make_unique<ExitEnemy>();

    currentState_ = states_[IEnemy::STATE::APPEAR].get();
}

Enemy::~Enemy()
{
    bodyPos_->UnRegisterObject();
}

void Enemy::Init()
{
    Json file = JsonFile::GetJsonFiles("Boss");
    characterState_.hps.maxHp = file["First"]["HP"];
    characterState_.hps.hp = characterState_.hps.maxHp;
    characterState_.isDead = false;
    characterState_.isHit = false;

    isAppear_ = false;
    isShotStart_ = false;
    isFootPreCollided_ = false;

    bodyPos_->Initialize();
    bodyPos_->SetScale({ 0.0f,0.0f,0.0f });
    bodyPos_->SetObjectName("GiantEnemy");
    bodyPos_->RegisterObject();
    bodyPos_->SetAnimation("Nod");
    bodyPos_->SetMaskVol(0.0f);

    poyoAnimTimer_ = 0.0f;

}

void Enemy::Draw(const Object3d::LightMode& lightMode)
{
    if (!isAppear_) { return; }

    bodyPos_->SetLightMode(lightMode);
    bodyPos_->Draw();

}

void Enemy::Update()
{

#ifdef USE_IMGUI  

    ImGui::Begin("Enemy");

    DebugUI::CheckCharacterState(characterState_, "Enemy");
    ImGui::Checkbox("isAppear", &isAppear_);

    for (auto& [name, collider] : colliders_) {

        auto& collided = collider.collider_->GetCollisionInfo().collided;

        if (ImGui::TreeNode(name.c_str())) {

            ImGui::Text("isCollided %d", collided ? "Yes" : "No");
            ImGui::TreePop();
        }

    }

    const char* phase[] = { "APPEAR", "ROUND", "SHOT","ALPHA_WALK","BEAM","IDLE","EXIT"};
    int currentPhase = static_cast<int>(currentState_->GetState());

    if (ImGui::Combo("CurrentPhase", &currentPhase, phase, IM_ARRAYSIZE(phase))) {
        ChangeState(static_cast<IEnemy::STATE>(currentPhase));
    };


    ImGui::End();

#endif // USE_IMGUI  

    if (!isAppear_) { return; }

    if (currentState_ != states_[currentState_->GetState()].get()) {
        //もしも現在のステートポインタが指定したポインタと一緒じゃなかったら
        //ステートを変更する
        ChangeState(currentState_->GetState());
    }

    // 呼び出す  
    currentState_->Update(this);

    HitUpdate();
    bodyPos_->UpdateAniTimer();
    bodyPos_->Update();

    //マトリックスの更新
    colliders_["EnemyFoot_L"].matrix_ = bodyPos_->GetWorldJointMatrix("foot_L");
    colliders_["EnemyFoot_R"].matrix_ = bodyPos_->GetWorldJointMatrix("foot_R");
    //頭の位置を仮入れ
    colliders_["EnemyHead"].matrix_ = bodyPos_->GetWorldJointMatrix("head");


    for (auto& [name, mat] : eyeMats_) {
        mat = bodyPos_->GetWorldJointMatrix(name);
    }

}

void Enemy::SoundFootStep(const SoundFactory::TAG tag)
{

    soundTimer_ -= TimeManager::DeltaTime();


    bool isCollided = false;

    for (auto& [name, collider] : colliders_) {

        //名前が目じゃないとき
        if (name == "EnemyHead") { continue; }

        if (collider.collider_->GetCollisionInfo().collided) {
            isCollided = true;
            break;

        }
    }

    if (isCollided && !isFootPreCollided_) {

        if (soundTimer_ <= 0.0f) {

            //距離によって変化させる
            float distance = Distance(*target_, bodyPos_->GetTranslate());
            distance /= 100.0f;
            distance = std::clamp(distance, 0.0f, 1.0f);
            Sound::PlaySE(tag, distance);
            soundTimer_ = kWalkFootstepInterval_;
        }
    }

    isFootPreCollided_ = isCollided;
}

void Enemy::OnCollision(Collider* collider)
{

    if (!isAppear_) { return; }

    //デバック用
    OnCollisionCollider();

    if (collider->GetCollisionAttribute() == CollisionTag::GetTag("PlayerBulletCold") || collider->GetCollisionAttribute() == CollisionTag::GetTag("PlayerBulletHot")) {
        //プレイヤーの弾の温度によって後で変更する
        if (!characterState_.isHit) {
            characterState_.isHit = true;

            Sound::PlaySE(SoundFactory::CRACKER);

            if (characterState_.hps.hp > 0) {
                characterState_.hps.hp--;
            }

            poyoAnimTimer_ = 0.0f;
        }

        if (collider->GetCollisionAttribute() == CollisionTag::GetTag("Player")) {

            Sound::PlaySE(SoundFactory::VOICE_Asobimasyo, 2.0f);

        }

    }
}
const Vector3 Enemy::GetInitScale()
{
    return { kScale_,kScale_,kScale_ };
}
Vector3 Enemy::GetToTarget()const
{
    if (target_) {
        return ToTargetVector(*target_, bodyPos_->GetWorldTransform().GetWorldPosition());
    }

    return { Math::ZERO };
}


void Enemy::Look()
{
    TransformAni::LookTarget(bodyPos_->GetWorldTransform(), *target_);
}

bool Enemy::PoyoPoyoUpdateAndGetEnd(const float& endTimer)
{
    poyoAnimTimer_ += TimeManager::DeltaTime();
    poyoAnimTimer_ = std::clamp(poyoAnimTimer_, 0.0f, endTimer);
    TransformAni::PoyoPoyo(bodyPos_->GetWorldTransform(), poyoAnimTimer_, kScale_);

    return (poyoAnimTimer_ == endTimer);

}

void Enemy::HitUpdate()
{
    //毎フレーム呼ばれる

    if (characterState_.isHit) {

        //ヒットしたらぽよぽよ終わったらヒットフラグを下げる
        if (PoyoPoyoUpdateAndGetEnd()) {
            characterState_.isHit = false;

            if (characterState_.hps.hp <= 0.0f) {
                characterState_.isDead = true;
            }
        }


    } else {
        if (currentState_->GetState() != IEnemy::STATE::APPEAR) {
            //出現時じゃないとき線形補間する
            LerpScale();
        }

    }
}

void Enemy::LerpScale()
{
    auto scale = bodyPos_->GetScale();
    scale = Lerp(Vector3{ scale }, { kScale_,kScale_,kScale_ }, 0.5f);
    bodyPos_->SetScale(scale);
}

void Enemy::RotateY(const float& startRotateY, const float& endRotateY, const float& timer)
{
    Vector3 rotate = { 0.0f };
    rotate.y = Easing::EaseInBack(startRotateY, endRotateY, timer);
    bodyPos_->SetRotate(rotate);
}

void Enemy::ChangeState(const IEnemy::STATE& state)
{
    currentState_->ChangeState(state);
    currentState_->Exit(this);    
    IEnemy* enemyState = states_[currentState_->GetState()].get();
    if (enemyState) {
        currentState_ = enemyState;
        currentState_->Enter(this);
    }
}
