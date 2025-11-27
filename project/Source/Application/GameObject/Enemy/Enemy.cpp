#include "Enemy.h"
#include"Model.h"
#include"ModelManager.h"
#include"Camera.h"
#include"Input.h"
#include"Sound.h"
#include"JsonFile.h"
#include"CollisionConfig.h"
#include"SphericalCoordinate.h"
#include"Collision.h"
#include"Easing.h"
#include"DebugUI.h"
#include"VibrateManager.h"
#define PI std::numbers::pi_v<float>
#define QUARTER_PI PI*0.25f
#define HALF_PI PI*0.5f
#define PI_2  PI*2.0f

Enemy::Enemy()
{
    //アクション
    UpdateActions_ = {
         {PHASE::LERP_ROUND_POS, std::bind(&Enemy::LerpRoundPos, this)},
         {PHASE::ROUND, std::bind(&Enemy::Round, this)},
         {PHASE::LERP_SQUARE_POS, std::bind(&Enemy::LerpSquarePos, this)},
         {PHASE::SQUARE, std::bind(&Enemy::SquareMove, this)},
         {PHASE::RANDOM_WALK, std::bind(&Enemy::RandomWalk, this)},
         {PHASE::FIREBALL, std::bind(&Enemy::Fireball, this)},
         {PHASE::FLOORCHANGEATTACK, std::bind(&Enemy::FloorChangeAttack, this)},
         {PHASE::TACKLE, std::bind(&Enemy::Tackle, this)},
         {PHASE::KNOCKBACK, std::bind(&Enemy::KnockBack, this)},
         {PHASE::PLAYER_HIT_BACK, std::bind(&Enemy::PlayerHitBack, this)},
         {PHASE::SHOCKWAVEATTACK, std::bind(&Enemy::ShockWaveAttack, this)},
         {PHASE::EXIT, std::bind(&Enemy::Exit, this)},
    };

    SwitchRandomAttackPhase_ = {
         {"First", std::bind(&Enemy::RandomAttackPhaseFirst, this)},
         {"Second", std::bind(&Enemy::RandomAttackPhaseSecond, this)},
         {"Third", std::bind(&Enemy::RandomAttackPhaseEnd, this)},
    };


    bodyPos_.Create();
    wingLPos_.Create();
    wingRPos_.Create();

    model_ = ModelManager::GetModel(ModelManager::ENEMY_BODY);

    bodyPos_.SetMesh(model_);
    wingLPos_.SetMesh(ModelManager::GetModel(ModelManager::ENEMY_WING_L));
    wingRPos_.SetMesh(ModelManager::GetModel(ModelManager::ENEMY_WING_R));
    wingLPos_.worldTransform_.Parent(bodyPos_.worldTransform_);
    wingRPos_.worldTransform_.Parent(bodyPos_.worldTransform_);

    bodyPos_.worldTransform_.scale_ = { kSize_,kSize_,kSize_ };

    wingLPos_.worldTransform_.translate_.x = -1.0f;
    wingRPos_.worldTransform_.translate_.x = 1.0f;

    model_->GetWaveData(0).direction = { 0.0f,0.0f,1.0f };
    model_->GetWaveData(0).amplitude = 0.05f;
    model_->GetWaveData(0).frequency = 8.0f;
    Init();

    //コライダーの設定
    SetRadius(kRadius_);
    SetCollisionAttribute(kCollisionEnemy);
    // 敵は「プレイヤー」と「プレイヤーの弾」と衝突したい
    SetCollisionMask(kCollisionPlayer | kCollisionPlayerBullet);
}

void Enemy::Init() {

    isShot_ = false;
    isBombShot_ = false;
    isWaveShot_ = false;
    isReqestClearFloor_ = false;
    isAttack_ = false;
    isPreAttack_ = false;

    isReqestClearFloor_ = false;

    actionCount_ = 0;

    currentState_ = "First";
    phase_ = PHASE::ROUND;

    //体についての項目
    bodyPos_.worldTransform_.Initialize();
    bodyPos_.worldTransform_.translate_.y = GetRadius();
    wingLPos_.worldTransform_.Initialize();
    wingRPos_.worldTransform_.Initialize();
    wingLPos_.worldTransform_.translate_.x = -1.0f;
    wingRPos_.worldTransform_.translate_.x = 1.0f;

    wingLPos_.worldTransform_.translate_.y = 0.3f;
    wingRPos_.worldTransform_.translate_.y = 0.3f;

    //球面座標系
    sphericalPos_ = { .radius = 0.0f,.azimuthal = 0.0f ,.polar = 0.0f };

    startPos_ = { 0.0f };
    endPos_ = { 0.0f };

    phaseTimer_ = 0.0f;
    poyoAnimTimer_ = 0.0f;

    //弾のクールタイム
    fireBallCoolTimer_ = 0.0f;

    endRotateY_ = 0.0f;
    startRotateY_ = 0.0f;
    roundSpeedY = 1.0f;

    wingTheta_ = 0.0f;

    InitState();

    model_->GetWaveData(0).time = 0.0f;
}

void Enemy::InitState()
{
    Json file = JsonFile::GetJsonFiles("Boss");
    //アニメーションタイマー
    phaseTime_ = file[currentState_]["phaseTime"];
    //ダメージ構造体
    damageStruct_.hps.maxHp = file[currentState_]["HP"];
    damageStruct_.hps.hp = damageStruct_.hps.maxHp;
    damageStruct_.hps.hpDecrease = file[currentState_]["hpDecrease"];
    damageStruct_.isHit = false;
    damageStruct_.isDead = false;
    //速度
    float velocity = file[currentState_]["velocity"];
    velocity_ = { velocity ,velocity ,velocity };

}


void Enemy::Draw(Camera& camera, const LightMode& lightMode)
{
    bodyPos_.SetLightMode(lightMode);
    wingLPos_.SetLightMode(lightMode);
    wingRPos_.SetLightMode(lightMode);

    bodyPos_.Draw(camera, kBlendModeNormal);
    wingLPos_.Draw(camera, kBlendModeNormal);
    wingRPos_.Draw(camera, kBlendModeNormal);

    ColliderDraw(camera);
}

void Enemy::Update()
{
    // とりあえずフェーズが最大になったら処理を終える  
    if (phase_ >= MAX_PHASE || phase_ < 0) {
        return;
    }

    UpdatePhaseTimer();
    // 呼び出す  
    UpdateActions_[phase_]();
    //フェーズごとに呼び出す関数をカエル
    SwitchPhase();

#ifdef _DEBUG
    if (Input::IsTriggerKey(DIK_Z)) { SetPhase(FLOORCHANGEATTACK); }
    if (Input::IsTriggerKey(DIK_X)) { SetPhase(SHOCKWAVEATTACK); }
    if (Input::IsTriggerKey(DIK_C)) { SetPhase(TACKLE); }
    if (Input::IsTriggerKey(DIK_V)) { SetPhase(FIREBALL); }
#endif

    damageStruct_.isHit = false;

    HitAnimation();
    model_->GetWaveData(0).time += InverseFPS * 4.0f;
    Winging(2.0f);

    bodyPos_.Update();
    wingLPos_.Update();
    wingRPos_.Update();

    ColliderUpdate();

    if (damageStruct_.hps.hp <= 0.0f) {
        SwitchState();
    }

#ifdef USE_IMGUI

    ImGui::Begin("Enemy");
    DebugUI::CheckDamageStruct(damageStruct_, "Boss");
    DebugUI::CheckObject3d(bodyPos_, "bodyPos");
    DebugUI::CheckObject3d(wingLPos_, "wingLPos");
    DebugUI::CheckObject3d(wingRPos_, "wingRPos");

    ImGui::Text("%s", currentState_.c_str());

    ImGui::Checkbox("isAttack", &isAttack_);
    ImGui::Checkbox("isPreAttack", &isPreAttack_);
    ImGui::SliderInt("actionCount", &actionCount_, 0, 3);
    ImGui::SliderFloat("phaseTimer", &phaseTimer_, 0.0f, 15.0f);
    ImGui::SliderFloat("phaseTime", &phaseTime_, 0.0f, 15.0f);
    DebugUI::CheckWaveData(model_->GetWaveData(0), "EnemyWaveData");

    ImGui::Text("toPlayer %f %f %f", playerLookDir_->x, playerLookDir_->y, playerLookDir_->z);
    Vector3 direction = endPos_ - startPos_;
    ImGui::Text("direction  %f %f %f", direction.x, direction.y, direction.z);
    float dot = Dot(*playerLookDir_, endPos_ - startPos_);
    ImGui::Text("Dot : %f", dot);
    ImGui::Text("%s",dot < 0.0f? "true":"false");
    ImGui::End();

    ImGui::Begin("Command");
    if (ImGui::TreeNode("EnemyCommand")) {
        ImGui::Text("Z : FloorChangeAttack");
        ImGui::Text("X : ShockWaveAttack");
        ImGui::Text("C : Rackle");
        ImGui::Text("V : FireBall");
        ImGui::TreePop();
    }
    ImGui::End();

#endif
}

Vector3 Enemy::GetWorldPosition()const
{
    return bodyPos_.worldTransform_.GetWorldPosition();
}

void Enemy::OnCollision(Collider* collider)
{
    //デバック用
    OnCollisionCollider();

    if (collider->GetCollisionAttribute() == kCollisionPlayerBullet) {

        if (damageStruct_.isHit) {
            return;
        }

        damageStruct_.isHit = true;
        Sound::PlaySE(Sound::DEFEAT_BOSS);
        
        VibrateManager::SetTime(0.5f, 2000, 2000);

        if (damageStruct_.hps.hp > 0) {
            damageStruct_.hps.hp -= damageStruct_.hps.hpDecrease;
        }
        poyoAnimTimer_ = 0.0f;
    }

    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        if (phase_ == TACKLE) {
            SetPhase(PLAYER_HIT_BACK);
        }
    }

}

void Enemy::Tackle()
{

    if (phaseTimer_ <= 3.0f) {

        if (phaseTimer_ <= 2.0f) {
            LookTarget(*playerPos_);
        }

        PoyoPoyo(3.0f);

    } else if (phaseTimer_ <= 3.7f) {
        float localTimer = (phaseTimer_ - 3.0f) / 0.7f;
        LerpScale();
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(startPos_, endPos_, localTimer);
        if (/*phaseTimer_ >= 6.0f && */Dot(*playerLookDir_, endPos_ - startPos_)<0.0f) {
            if (damageStruct_.isHit) {
                Sound::PlayOriginSE(Sound::BOSS_TACKLE);
                VibrateManager::SetTime(1.0f, 2000, 2000);
                SetPhase(KNOCKBACK);
            }
        }


    } else if (phaseTimer_ <= 4.7f) {
        float localTimer = (phaseTimer_ - 3.7f) * 0.5f;
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(endPos_, startPos_, localTimer);
    } else if (phaseTimer_ <= 5.7f) {
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(bodyPos_.worldTransform_.translate_, startPos_, 0.5f);
    } else {
        phaseTimer_ = phaseTime_;
    }

}

void Enemy::PlayerHitBack()
{

    if (phaseTimer_ == 0.0f) {
        LookTarget(*playerPos_);
    }

    if (phaseTimer_ <= 0.125f) {
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(endPos_, startPos_, phaseTimer_);
    } else if (phaseTimer_ <= 2.0f) {
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, startPos_, 0.05f);

    } else {
        phaseTimer_ = phaseTime_;
    }
}

void Enemy::KnockBack()
{

    if (phaseTimer_ == 0.0f) {
        LookTarget(*playerPos_);
    }

    if (phaseTimer_ <= 0.125f) {
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(bodyPos_.worldTransform_.translate_, startPos_, 0.05f);
    } else {

        if (phaseTimer_ <= 0.75f) {
            SpinBody();
            Sound::PlayOriginSE(Sound::STUN);
        } else {
            LerpSpinOriginBody();
            bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, startPos_, 0.05f);
        }
    }

    if (phaseTimer_ > 2.0f) {
        phaseTimer_ = phaseTime_;
        /*  SetPhase(LERP_ROUND_POS);*/
    }


}


void Enemy::LerpRoundPos()
{
    sphericalPos_.polar = Lerp(sphericalPos_.polar, 0.0f, 0.05f);
    bodyPos_.worldTransform_.translate_ = TransformCoordinate(sphericalPos_);
    if (fabs(sphericalPos_.polar) <= QUARTER_PI) {
        SetPhase(ROUND);
    }
}

void Enemy::LerpSquarePos()
{

    if (phaseTimer_ <= 1.0f) {
        bodyPos_.worldTransform_.rotate_.y = Lerp(bodyPos_.worldTransform_.rotate_.y, PI, 0.1f);
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, { 0.0f,kSize_,0.0f }, phaseTimer_);
    } else if (phaseTimer_ <= kLerpSquareTime_) {
        LerpPos();
    } else {
        SetPhase(SQUARE);
    }

}

void Enemy::SquareMove()
{

    float loopedTime = std::fmod(phaseTimer_, 4.0f);
    Vector3 endPos = { 0.0f,0.0f,0.0f };
    if (loopedTime <= 1.0f) {
        endPos = { 6.0f,kRadius_,6.0f };
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, endPos, kSquareMoveSpeed_);
    } else if (loopedTime <= 2.0f) {
        endPos = { 6.0f,kRadius_,-6.0f };
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, endPos, kSquareMoveSpeed_);
    } else if (loopedTime <= 3.0f) {
        endPos = { -6.0f,kRadius_,-6.0f };
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, endPos, kSquareMoveSpeed_);
    } else if (loopedTime <= 4.0f) {
        endPos = { -6.0f,kRadius_,6.0f };
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, endPos, kSquareMoveSpeed_);
    }

    LookTarget(endPos);
}


void Enemy::RandomWalk()
{
}

void Enemy::LerpPos()
{
    bodyPos_.worldTransform_.rotate_.y = Lerp(bodyPos_.worldTransform_.rotate_.y, PI, 0.1f);
    bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, { 0.0f,0.0f,6.0f }, 0.05f);
}

void Enemy::RandomMovePhase()
{

    //int randNum = rand() % 4;

    int randNum = rand() % 2;

    switch (randNum)
    {
    case 0:
        SetPhase(LERP_ROUND_POS);
        break;
    case 1:
        SetPhase(LERP_SQUARE_POS);
        break;
    }
}

void Enemy::RandomAttackPhaseFirst()
{
    int randNum = rand() % 2;
    switch (randNum)
    {
    case 0:
        SetPhase(TACKLE);
        break;
    case 1:
        SetPhase(FIREBALL);
        break;
    }
}

void Enemy::RandomAttackPhaseSecond()
{
    int randNum = rand() % 2;
    switch (randNum)
    {
    case 0:
        SetPhase(TACKLE);
        break;
    case 1:
        SetPhase(FIREBALL);
        break;
    case 2:
        SetPhase(FLOORCHANGEATTACK);
        break;
    }
}

void Enemy::RandomAttackPhaseEnd()
{
    int randNum = rand() % 2;

    switch (randNum)
    {
    case 0:
        RandomAttackPhaseSecond();
        break;
    case 1:
        SetPhase(SHOCKWAVEATTACK);
        break;
    }

}



void Enemy::SwitchState()
{



    Sound::PlaySE(Sound::BOSS_HEAL);
    isReqestClearFloor_ = true;

    if (currentState_ == "First") {
        currentState_ = "Second";
    } else if (currentState_ == "Second") {
        currentState_ = "Third";
    } else if (currentState_ == "Third") {
        
        damageStruct_.isDead = true;
        
        //currentState_ = "First";
    }


    InitState();
}

void Enemy::SetPhase(PHASE phase)
{

    phaseTimer_ = 0.0f;
    phase_ = phase;
    poyoAnimTimer_ = 0.0f;

    //フェーズがーLERP_ROUND_POSだったらここでreturn
    if (phase_ == LERP_ROUND_POS || phase_ == LERP_SQUARE_POS || phase_ == KNOCKBACK || phase_ == PLAYER_HIT_BACK) {
        sphericalPos_ = TransformCoordinate(bodyPos_.worldTransform_.translate_);
        return;
    }

    if (phase_ == FIREBALL || phase_ == KNOCKBACK) {
        startRotateY_ = bodyPos_.worldTransform_.rotate_.y;
        endRotateY_ = startRotateY_ + PI_2;
    }

    //フェーズが攻撃かどうかを判断する
    isAttack_ = (phase_ < LERP_ROUND_POS) ? true : false;

    //前と同じだったらアクションカウントを足す
    if (isAttack_ == isPreAttack_) {
        actionCount_++;
    } else {
        actionCount_ = 0;
    }

    //前の攻撃フラグに代入する
    isPreAttack_ = isAttack_;

}

void Enemy::SwitchPhase()
{
    if (phaseTimer_ == phaseTime_) {
        if (actionCount_ < 1) {
            int randNum = rand() % 2;
            if (randNum == 0) {
                RandomMovePhase();
            } else {
                SwitchRandomAttackPhase_[currentState_]();
            }

        } else {
            if (isAttack_) {
                RandomMovePhase();
            } else {
                SwitchRandomAttackPhase_[currentState_]();
            }
        }

    }
}


void Enemy::Round()
{

    sphericalPos_.radius = Lerp(sphericalPos_.radius, enemyRoundCircle_.radius, kSphericalLerpSpeed_);


    sphericalPos_.polar += InverseFPS * roundSpeedY;

    if (sphericalPos_.polar > HALF_PI || sphericalPos_.polar < -HALF_PI) {
        roundSpeedY *= -1.0f;
    }

    LookTarget(*playerPos_);

    bodyPos_.worldTransform_.translate_ = TransformCoordinate(sphericalPos_);
    bodyPos_.worldTransform_.translate_.y = GetRadius();
}


void Enemy::Fireball()
{
    if (phaseTimer_ <= 1.0f) {
        RotateY(phaseTimer_);
        fireBallCoolTimer_ = 0.0f;
    } else {

        LookTarget(*playerPos_);

        fireBallCoolTimer_ += InverseFPS;

        if (fireBallCoolTimer_ > 1.5f) {
            fireBallCoolTimer_ = 0.0f;
            isShot_ = true;
        }
    }
}

void Enemy::FloorChangeAttack()
{
    LerpPos();
    LerpScale();

    if (phaseTimer_ <= 1.0f) {
        RotateY(phaseTimer_);

    } else if (phaseTimer_ <= 1.5f) {
        if (!isBombShot_) {
            isBombShot_ = true;
        }
        LookTarget(*target_);
    }


}
void Enemy::ShockWaveAttack()
{
    if (phaseTimer_ <= 1.0f) {
        LookTarget(*playerPos_);
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, *target_, 0.05f);
    } else if (phaseTimer_ <= 1.2f) {
        if (!isWaveShot_) {
            isWaveShot_ = true;
        }
    }
}
void Enemy::Exit()
{
    if (phaseTimer_ <= 4.0f) {
        SpinBody();
    } else {
        LerpSpinOriginBody();
    }
}


void Enemy::UpdatePhaseTimer()
{
    if (phaseTimer_ < phaseTime_) {
        phaseTimer_ += InverseFPS;
    } else {
        phaseTimer_ = phaseTime_;
    }

}

void Enemy::LookTarget(Vector3& target)
{
    startPos_ = GetWorldPosition();
    endPos_ = target;
    Vector3 direction = endPos_ - startPos_;
    bodyPos_.worldTransform_.rotate_.y = std::atan2(direction.x, direction.z); // Y軸回転（ラジアン）

}

void Enemy::PoyoPoyo(const float& endTimer)
{

    poyoAnimTimer_ += InverseFPS;
    poyoAnimTimer_ = std::clamp(poyoAnimTimer_, 0.0f, endTimer);

    float theta = PI * 10.0f * poyoAnimTimer_;
    bodyPos_.worldTransform_.scale_.x = kSize_ + cos(theta) * 0.5f;
    bodyPos_.worldTransform_.scale_.y = kSize_ + sin(theta) * 0.5f;

}

void Enemy::HitAnimation()
{
    if (poyoAnimTimer_ < kPoyoAnimeTime_) {
        PoyoPoyo(kPoyoAnimeTime_);
    } else {
        LerpScale();
    }
}

void Enemy::LerpScale()
{
    bodyPos_.worldTransform_.scale_ = Lerp(Vector3{ bodyPos_.worldTransform_.scale_ }, { kSize_,kSize_,kSize_ }, 0.5f);
}

void Enemy::RotateY(const float& timer)
{
    bodyPos_.worldTransform_.rotate_.y = Easing::EaseInBack(startRotateY_, endRotateY_, timer);

}

void Enemy::SpinBody()
{
    float localTimer = (phaseTimer_ - 0.125f) / (0.75f - 0.125f);
    float theta = PI * 3.0f * localTimer; // 回転の速さを調整
    bodyPos_.worldTransform_.rotate_.z = sinf(theta);
    bodyPos_.worldTransform_.rotate_.x = cosf(theta);
}

void Enemy::LerpSpinOriginBody()
{
    bodyPos_.worldTransform_.rotate_.z = Lerp(bodyPos_.worldTransform_.rotate_.z, 0.0f, 0.5f);
    bodyPos_.worldTransform_.rotate_.x = Lerp(bodyPos_.worldTransform_.rotate_.x, 0.0f, 0.5f);
}

void Enemy::Winging(const float& speed)
{
    wingTheta_ += std::numbers::pi_v<float>*InverseFPS * speed;
    wingLPos_.worldTransform_.rotate_.z = sinf(wingTheta_);
    wingRPos_.worldTransform_.rotate_.z = -sinf(wingTheta_);
}
