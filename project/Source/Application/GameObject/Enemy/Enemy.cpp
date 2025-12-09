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
#include "MatsumotoObj/SceneStaticValue.h"
#include "MatsumotoObj/GameSceneObj/HealItemSpawner.h"
#include "MatsumotoObj/GameSceneObj/Data/MapData.h"
#include "MatsumotoObj/GameSceneObj/AttackAreaEmitter.h"

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
         {PHASE::RANDOM_FLOORCHANG_ATTACK, std::bind(&Enemy::RandomFloorChangAttack, this)},
         {PHASE::FIREBALL, std::bind(&Enemy::Fireball, this)},
         {PHASE::FLOORCHANGEATTACK, std::bind(&Enemy::FloorChangeAttack, this)},
         {PHASE::TACKLE, std::bind(&Enemy::Tackle, this)},
         {PHASE::KNOCKBACK, std::bind(&Enemy::KnockBack, this)},
         {PHASE::PLAYER_HIT_BACK, std::bind(&Enemy::PlayerHitBack, this)},
         {PHASE::SHOCKWAVEATTACK, std::bind(&Enemy::ShockWaveAttack, this)},
         {PHASE::EXIT, std::bind(&Enemy::Exit, this)},
    };

    //フェーズ抽選の関数を入れる
    SwitchRandomAttackPhase_ = {
         {"First", std::bind(&Enemy::RandomAttackPhaseFirst, this)},
         {"Second", std::bind(&Enemy::RandomAttackPhaseSecond, this)},
         {"Third", std::bind(&Enemy::RandomAttackPhaseEnd, this)},
    };

    //コライダーの設定
    SetRadius(kRadius_);
    SetCollisionAttribute(kCollisionEnemy);
    // 敵は「プレイヤー」と「プレイヤーの弾」と衝突したい
    SetCollisionMask(kCollisionPlayer | kCollisionPlayerBullet);

    //それぞれのオブジェクト座標を入れる
    bodyPos_.Create();
    wingLPos_.Create();
    wingRPos_.Create();

    //体のモデルを取得する
    models_[BODY] = ModelManager::GetModel(ModelManager::ENEMY_BODY);
    models_[WING_L] = ModelManager::GetModel(ModelManager::ENEMY_WING_L);
    models_[WING_R] = ModelManager::GetModel(ModelManager::ENEMY_WING_R);
    bodyPos_.GetWaveData(0).direction = { 0.0f,0.0f,1.0f };
    bodyPos_.GetWaveData(0).amplitude = 0.05f;
    bodyPos_.GetWaveData(0).frequency = 8.0f;

    //各々のトランスフォームにメッシュを入れる
    bodyPos_.SetMesh(models_[BODY]);
    wingLPos_.SetMesh(models_[WING_L]);
    wingRPos_.SetMesh(models_[WING_R]);
    //体の位置にペアレント化する
    wingLPos_.worldTransform_.Parent(bodyPos_.worldTransform_);
    wingRPos_.worldTransform_.Parent(bodyPos_.worldTransform_);


    enemyBeak_ = std::make_unique<EnemyBeak>();
    enemyBeak_->beak_.Parent(bodyPos_.worldTransform_);

    //初期化
    Init();

    Json file = JsonFile::GetJsonFiles("Boss");
    totalHPs_.maxHp = 0;
    totalHPs_.maxHp = file["First"]["HP"];
    totalHPs_.maxHp += file["Second"]["HP"];
    totalHPs_.maxHp += file["Third"]["HP"];


}

void Enemy::Init() {

    //フラグの初期化
    isShot_ = false;
    isBombShot_ = false;
    isWaveShot_ = false;
    isWavePosSelect_ = false;
    isKnockBackEmit_ = false;
    isKnockBack_ = false;

    isAttack_ = false;
    isPreAttack_ = false;
    isFeint_ = false;
    isLeathalVec_ = false;

    isReqestClearFloor_ = false;
    isReqestOverheadView_ = false;

    isFloorBring_ = false;
    isGoToRanDomFloor_ = false;

    //アクションカウントの初期化　何回同じタイプの行動をしたかを記録
    actionCount_ = 0;

    currentState_ = "First";
    if (SceneStaticValue::bossRound >= 2) {
        currentState_ = "Second";
    }
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

    enemyBeak_->Initialize();

    for (auto& [type, model] : models_) {
        model->ResetTextureHandle();
    }


    //球面座標系
    sphericalPos_ = { .radius = 0.0f,.azimuthal = 0.0f ,.polar = 0.0f };

    startPos_ = { 0.0f };
    endPos_ = { 0.0f };

    phaseTimer_ = 0.0f;
    poyoAnimTimer_ = 0.0f;

    //弾のクールタイム
    fireBallCoolTimer_ = 0.0f;

    startRotateY_ = 0.0f;
    endRotateY_ = 0.0f;
    roundSpeedY = 1.0f;

    wingTheta_ = 0.0f;

    overKillCount = 0;
    totalHPs_.hp = totalHPs_.maxHp;

    InitState();

    bodyPos_.GetWaveData(0).time = 0.0f;

	fireBallAttackAreaID_ = UINT32_MAX;
	tackleAttackAreaID_ = UINT32_MAX;

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
    damageStruct_.isInvincible = false;
    //速度
    float velocity = file[currentState_]["velocity"];
    velocity_ = { velocity ,velocity ,velocity };
}


void Enemy::Draw(Camera& camera)
{
    bodyPos_.Draw(camera, kBlendModeNormal);
    wingLPos_.Draw(camera, kBlendModeNormal);
    wingRPos_.Draw(camera, kBlendModeNormal);
    enemyBeak_->Draw(camera);
    ColliderDraw(camera);
}

void Enemy::Update()
{
    if (IsOverKill()) {
        return;
    }

    // とりあえずフェーズが最大になったら処理を終える  
    if (phase_ >= MAX_PHASE || phase_ < 0) {
        return;
    }

    UpdatePhaseTimer();
    //フェーズごとに呼び出す関数をカエル
    SwitchPhase();

#ifdef _DEBUG
    if (Input::IsTriggerKey(DIK_Z)) { SetPhase(FLOORCHANGEATTACK); }
    if (Input::IsTriggerKey(DIK_X)) { SetPhase(SHOCKWAVEATTACK); }
    if (Input::IsTriggerKey(DIK_C)) { SetPhase(TACKLE); }
    if (Input::IsTriggerKey(DIK_V)) { SetPhase(FIREBALL); }


    if (Input::IsTriggerKey(DIK_M)) { SetPhase(RANDOM_FLOORCHANG_ATTACK); }

    if (Input::IsTriggerKey(DIK_B)) { SetPhase(LERP_ROUND_POS); }
    if (Input::IsTriggerKey(DIK_N)) { SetPhase(LERP_SQUARE_POS); }

    if (Input::IsTriggerKey(DIK_L)) { SetPhase(EXIT); }
#endif
    // 呼び出す  
    UpdateActions_[phase_]();



    damageStruct_.isHit = false;
    isKnockBack_ = false;

    HitAnimation();
    bodyPos_.GetWaveData(0).time += InverseFPS * 4.0f;

    if (!damageStruct_.isDead) {
        Winging(2.0f);
    }

    bodyPos_.Update();
    wingLPos_.Update();
    wingRPos_.Update();

    enemyBeak_->Update();

    ColliderUpdate();

    if (damageStruct_.hps.hp <= 0.0f) {
        SwitchState();
    }

#ifdef USE_IMGUI

    ImGui::Begin("Enemy");

    ImGui::Checkbox("isFloorBring_", &isFloorBring_);
    ImGui::Checkbox("isGoToRanDomFloor_", &isGoToRanDomFloor_);


    DebugUI::CheckDamageStruct(damageStruct_, "Boss");
    DebugUI::CheckObject3d(bodyPos_, "bodyPos");
    DebugUI::CheckObject3d(wingLPos_, "wingLPos");
    DebugUI::CheckObject3d(wingRPos_, "wingRPos");

    ImGui::SliderFloat3("sphericalPos_", &sphericalPos_.radius, -10.0f, 10.0f);
    DebugUI::CheckWaveData(bodyPos_.GetWaveData(0), "EnemyWaveData");

    ImGui::Text("%s", currentState_.c_str());

    if (ImGui::TreeNode("SelectBossState")) {

        const char* states[] = { "First", "Second", "Third" };
        int current = 0;

        if (ImGui::Combo("BossState", &current, states, IM_ARRAYSIZE(states))) {
            currentState_ = states[current % 3];
        };

        ImGui::TreePop();
    }
    ImGui::Checkbox("isSelectRandomPhase", &isSelectRandomPhase_);
    ImGui::SliderFloat("phaseTimer", &phaseTimer_, 0.0f, 15.0f);
    ImGui::SliderFloat("phaseTime", &phaseTime_, 0.0f, 15.0f);

    ImGui::Checkbox("isAttack", &isAttack_);
    ImGui::Checkbox("isPreAttack", &isPreAttack_);
    ImGui::SliderInt("actionCount", &actionCount_, 0, 3);

    ImGui::Text("toPlayer %f %f %f", playerLookDir_->x, playerLookDir_->y, playerLookDir_->z);
    Vector3 direction = endPos_ - startPos_;
    ImGui::Text("direction  %f %f %f", direction.x, direction.y, direction.z);
    float dot = Dot(*playerLookDir_, endPos_ - startPos_);
    ImGui::Text("Dot : %f", dot);
    ImGui::Text("%s", dot < 0.0f ? "true" : "false");
    ImGui::End();

    ImGui::Begin("Command");
    if (ImGui::TreeNode("EnemyCommand")) {
        ImGui::Text("Z : FloorChangeAttack");
        ImGui::Text("X : ShockWaveAttack");
        ImGui::Text("C : Rackle");
        ImGui::Text("V : FireBall");

        ImGui::Text("B : ROUND_WALK");
        ImGui::Text("N : SQUARE_WALK");
        ImGui::Text("M : RANDOM_FLOORCHANG_ATTACK");
        ImGui::Text("L : EXIT");
        ImGui::TreePop();
    }



    ImGui::End();

#endif
    if (currentState_ == "First") {
        SceneStaticValue::bossRound = 1;
    } else if (currentState_ == "Second") {
        SceneStaticValue::bossRound = 2;
    } else if (currentState_ == "Third") {
        SceneStaticValue::bossRound = 3;
    }
}

Vector3 Enemy::GetWorldPosition()const
{
    return bodyPos_.worldTransform_.GetWorldPosition();
}

void Enemy::OnCollision(Collider* collider)
{
    //デバック用
    OnCollisionCollider();

    if (isFaseChange_) {
        // フェーズチェンジ中はHP2/3以下のダメージを受け付けない
        if ((damageStruct_.hps.maxHp / 3) * 2 >= damageStruct_.hps.hp) {
            return;
        }
    }

    if (collider->GetCollisionAttribute() == kCollisionPlayerBullet) {

        VibrateManager::SetTime(0.5f, 2000, 2000);

        damageStruct_.isHit = true;

        if (damageStruct_.hps.hp > 0) {
            totalHPs_.hp -= damageStruct_.hps.hpDecrease;
            damageStruct_.hps.hp -= damageStruct_.hps.hpDecrease;
        }
        poyoAnimTimer_ = 0.0f;

        if (damageStruct_.isDead) {
            overKillCount++;
            Sound::PlayOriginSE(Sound::kStun);
            Sound::PlayOriginSE(Sound::kDefeatBoss);
        } else {

            Sound::PlaySE(Sound::kDefeatBoss);
        }
    }

    if (collider->GetCollisionAttribute() == kCollisionPlayer) {
        if (phase_ == TACKLE) {
            SetPhase(PLAYER_HIT_BACK);
        }
    }

}
void Enemy::ForceSetBossRound(uint32_t round)
{
    if (round == 1) {
        currentState_ = "First";
    } else if (round == 2) {
        currentState_ = "Second";
    } else if (round >= 3) {
        currentState_ = "Third";
    }
}
void Enemy::LeathalMoveUpdate() {
    if (!isLeathalVec_) {
        isLeathalVec_ = true;
        velocity_ = { 5.0f,30.0f,350.0f };

    }

    velocity_.y -= 9.8f * 0.016f;
    bodyPos_.worldTransform_.translate_ += velocity_ * 0.016f;
    velocity_ *= 0.98f;

    bodyPos_.worldTransform_.rotate_.x += 0.3f;
    bodyPos_.worldTransform_.rotate_.y += 0.2f;
    bodyPos_.worldTransform_.rotate_.z += 0.1f;

    bodyPos_.Update();
    wingLPos_.Update();
    wingRPos_.Update();
}

void Enemy::Tackle()
{

    if (phaseTimer_ == 0.0f) {
        if (damageStruct_.hps.hp > 10.0f) {
            int random = rand() % 3;
            //1/3の確率でタックルしない
            if (random == 0) {
                // 必ずタックルするようにfalseにします(ジェイムズ修正)
                isFeint_ = false;
            } else {
                isFeint_ = false;
            }
        }
    }

    if (phaseTimer_ <= kTacklePoyoTime_) {

        if (phaseTimer_ <= kTackleLookTime_) {
            LookTargetAndSetPos(*playerPos_);
        }

        PoyoPoyo(kTacklePoyoTime_);

		if (tackleAttackAreaID_ == UINT32_MAX) {
			tackleAttackAreaID_ = AttackAreaEmitter::GetInstance().EmitSquareForm(
				bodyPos_.worldTransform_.GetWorldPosition(),
				Vector2(1.0f,1.0f),
                Vector2(3.0f,15.0f), kTacklePoyoTime_);
		}

        if (tackleAttackAreaID_ != UINT32_MAX) {
            AttackAreaEmitter::GetInstance().ZmRotateEffect(
                tackleAttackAreaID_,
                bodyPos_.worldTransform_.GetWorldPosition(),
                Vector2(cosf(bodyPos_.worldTransform_.rotate_.y), sinf(bodyPos_.worldTransform_.rotate_.y)),
                Vector2(1.0f, 15.0f));
        }

    } else if (phaseTimer_ <= kTackleInterval_) {

    } else if (phaseTimer_ <= kTackleGoPlayerTime_) {

        float localTimer = (phaseTimer_ - kTackleInterval_) / (kTackleGoPlayerTime_ - kTackleInterval_);

        if (localTimer > 0.1f) {
            if (isFeint_) {
                endPos_ = bodyPos_.worldTransform_.translate_;
            }
        }

        LerpScale();

        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(startPos_, endPos_, localTimer);

        if (!isFeint_) {
            if (Dot(*playerLookDir_, endPos_ - startPos_) < 0.0f) {
                if (damageStruct_.isHit) {

                    if (!isKnockBack_) {
						tackleAttackAreaID_ = UINT32_MAX;
                        isKnockBack_ = true;
                        isKnockBackEmit_ = true;
                        LookTargetNormal(*playerPos_);

                        Sound::PlayOriginSE(Sound::kBossTackle);
                        if (damageStruct_.hps.hp >= kKnockBackDamage_) {

                            totalHPs_.hp -= kKnockBackDamage_;
                            damageStruct_.hps.hp -= kKnockBackDamage_;
                        }
                        VibrateManager::SetTime(1.0f, 2000, 2000);

                        SetPhase(KNOCKBACK);
                    }


                }
            }
        }

    } else if (phaseTimer_ <= kTackleEndingLagTime_) {

    } else if (phaseTimer_ <= kTackleBackTime_) {
        float localTimer = (phaseTimer_ - kTackleEndingLagTime_) * 0.5f;//半減した速さで行う
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(endPos_, startPos_, localTimer);
    } else if (phaseTimer_ <= kTackleInitStartTime_) {
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(bodyPos_.worldTransform_.translate_, startPos_, 0.5f);
    }else {
        isSelectRandomPhase_ = true;
    }

}

void Enemy::PlayerHitBack()
{

    if (phaseTimer_ == 0.0f) {
        LookTargetAndSetPos(*playerPos_);
    }

    if (phaseTimer_ <= kHitBackTime_) {
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(endPos_, startPos_, phaseTimer_);
    } else if (phaseTimer_ <= kHitBackIntervalTime_) {

    } else if (phaseTimer_ <= kPlayerHitBackTime_) {
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, startPos_, 0.05f);
    } else {
        isSelectRandomPhase_ = true;
    }
}

void Enemy::KnockBack()
{

    if (phaseTimer_ <= kHitBackTime_) {
        bodyPos_.worldTransform_.translate_ = Easing::EaseOutBack(bodyPos_.worldTransform_.translate_, startPos_, 0.05f);
    } else if (phaseTimer_ <= kKnockBackSpinTime_) {

        SpinBody();

        if (phaseTimer_ >= kKnockBackSpinTime_ - 0.5f) {
            isKnockBackEmit_ = false;
        }

    } else if (phaseTimer_ < kKnockBackMaxTime_) {
        LerpSpinOriginBodyXZ();
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, startPos_, 0.05f);
    } else {
        isSelectRandomPhase_ = true;
    }

}


void Enemy::LerpRoundPos()
{

    sphericalPos_.polar = Lerp(sphericalPos_.polar, 0.0f, 0.05f);
    sphericalPos_.radius = Lerp(sphericalPos_.radius, kRoundRadius_, 0.05f);

    bodyPos_.worldTransform_.translate_ = TransformCoordinate(sphericalPos_);
    if (fabs(sphericalPos_.polar) <= QUARTER_PI && sphericalPos_.radius >= kRoundRadius_ - 2.0f) {
        SetPhase(ROUND);
    }
}

void Enemy::LerpSquarePos()
{

    if (phaseTimer_ <= kLerpSquareInitPosTime_) {
        LerpPos({ 0.0f,kLerpSquareStartPosY_,0.0f }, 0.1f);
        LerpRotateY(PI, 0.1f);
        LerpSpinOriginBodyXZ();
    } else if (phaseTimer_ <= kLerpSquareTime_) {
        LerpPos({ 0.0f,kLerpSquareEndPosY_,6.0f }, 0.1f);
    } else {
        isFaseChange_ = false;
        SetPhase(SQUARE);
    }

}

void Enemy::SquareMove()
{

    float loopedTime = std::fmod(phaseTimer_, 4.0f * kSquareMoveInterval_);

    Vector3 endPos = { 0.0f,0.0f,0.0f };
    if (loopedTime <= 1.0f * kSquareMoveInterval_) {
        endPos = { 6.0f,kRadius_,6.0f };
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, endPos, kSquareMoveSpeed_);
    } else if (loopedTime <= 2.0f * kSquareMoveInterval_) {
        endPos = { 6.0f,kRadius_,-6.0f };
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, endPos, kSquareMoveSpeed_);
    } else if (loopedTime <= 3.0f * kSquareMoveInterval_) {
        endPos = { -6.0f,kRadius_,-6.0f };
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, endPos, kSquareMoveSpeed_);
    } else if (loopedTime <= 4.0f * kSquareMoveInterval_) {
        endPos = { -6.0f,kRadius_,6.0f };
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, endPos, kSquareMoveSpeed_);
    }

    LookTargetNormal(endPos);

    if (phaseTimer_ >= kSquareMoveMaxTime_) {
        isSelectRandomPhase_ = true;
    }
}

void Enemy::LerpRotateY(const float& endRadius, const float& lerRotateSpeed)
{
    bodyPos_.worldTransform_.rotate_.y = Lerp(bodyPos_.worldTransform_.rotate_.y, endRadius, lerRotateSpeed);
}

void Enemy::LerpPos(const Vector3& endPos, const float& lerpPosSpeed)
{
    bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, endPos, lerpPosSpeed);
}

void Enemy::SwitchState()
{
    isReqestOverheadView_ = false;
    if (currentState_ == "First") {
        currentState_ = "Second";
        isFaseChange_ = true;
    } else if (currentState_ == "Second") {
        Vector3 spawnPos = { 0.0f ,10.0f, 0.0f };
        HealItemSpawner::Instance().SpawnHealItem(spawnPos);

        currentState_ = "Third";
        isFaseChange_ = true;

        for (auto& [type, model] : models_) {
            model->SetTextureHandle(Texture::ENEMY_TEXTURE2);
        }
    } else if (currentState_ == "Third") {
        damageStruct_.isDead = true;
        damageStruct_.hps.maxHp = kMaxOverKillCount;
        damageStruct_.hps.hp = kMaxOverKillCount;
    }

    //回転を初期化
    bodyPos_.worldTransform_.rotate_.z = 0.0f;

    if (damageStruct_.isDead) {
        if (phase_ != EXIT) {
            Sound::PlaySE(Sound::kBossDamage02);
            SetPhase(EXIT);
        }

        return;
    }

    isReqestClearFloor_ = true;

    Sound::PlaySE(Sound::kBossHeal);
    SetPhase(LERP_SQUARE_POS);

    InitState();
}

void Enemy::SetPhase(PHASE phase)
{

    phaseTimer_ = 0.0f;
    phase_ = phase;
    poyoAnimTimer_ = 0.0f;

    //フェーズがーLERP_ROUND_POSだったらここでreturn
    if (phase_ == LERP_ROUND_POS || phase_ == LERP_SQUARE_POS
        || phase_ == KNOCKBACK || phase_ == PLAYER_HIT_BACK) {
        sphericalPos_ = TransformCoordinate(bodyPos_.worldTransform_.translate_);
        return;
    }

    if (phase_ == FIREBALL || phase_ == KNOCKBACK) {
        //くるっと一回転するよ
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
    if (isSelectRandomPhase_) {
        isSelectRandomPhase_ = false;

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

    sphericalPos_.radius = Lerp(sphericalPos_.radius, kRoundRadius_, kSphericalLerpSpeed_);
    sphericalPos_.azimuthal = Lerp(sphericalPos_.azimuthal, 0.0f, 0.01f);

    sphericalPos_.polar += InverseFPS * roundSpeedY;

    if (sphericalPos_.polar > HALF_PI || sphericalPos_.polar < -HALF_PI) {
        roundSpeedY *= -1.0f;
    }

    LookTargetNormal(*playerPos_);

    bodyPos_.worldTransform_.translate_ = TransformCoordinate(sphericalPos_);
    bodyPos_.worldTransform_.translate_.y = GetRadius();

    if (phaseTimer_ >= phaseTime_) {
        isSelectRandomPhase_ = true;
    }
}


void Enemy::Fireball()
{
    if (phaseTimer_ <= kFireBallRotateTime_) {
        RotateY(phaseTimer_);
        fireBallCoolTimer_ = 0.0f;

        //yを戻す
        bodyPos_.worldTransform_.translate_.y = Lerp(bodyPos_.worldTransform_.translate_.y, kRadius_, 0.1f);
    } else {

        LookTargetNormal(*playerPos_);
        if (fireBallAttackAreaID_ != UINT32_MAX) {
            AttackAreaEmitter::GetInstance().ZmRotateEffect(
                fireBallAttackAreaID_,
                bodyPos_.worldTransform_.GetWorldPosition(),
                Vector2(cosf(bodyPos_.worldTransform_.rotate_.y), sinf(bodyPos_.worldTransform_.rotate_.y)),
                Vector2(1.0f, 15.0f));
        }

        fireBallCoolTimer_ += InverseFPS;

        if (fireBallCoolTimer_ > kFireBallMaxCoolTime_) {
            fireBallCoolTimer_ = 0.0f;
            isShot_ = true;
            fireBallAttackAreaID_ = UINT32_MAX;
        }

        if (fireBallAttackAreaID_ == UINT32_MAX) {
			fireBallAttackAreaID_ = AttackAreaEmitter::GetInstance().EmitSquareForm(
				bodyPos_.worldTransform_.GetWorldPosition(),
				Vector2(sinf(bodyPos_.worldTransform_.rotate_.y), cosf(bodyPos_.worldTransform_.rotate_.y)),
                Vector2(1.0f,15.0f), kFireBallMaxCoolTime_
            );
        }
    }

    if (phaseTimer_ >= kFireBallPhaseMaxTime_) {
        isSelectRandomPhase_ = true;
        fireBallAttackAreaID_ = UINT32_MAX;
    }

}

void Enemy::FloorChangeAttack()
{
    if (phaseTimer_ <= kFloorAttackPosMoveTime_) {
        isReqestOverheadView_ = true;
        LerpScale();
        LerpPos({ 0.0f,kRadius_,6.0f }, 0.1f);
        LerpRotateY(PI, 0.3f);
    } else if (phaseTimer_ <= kFloorBombShotTime_) {

        if (!isBombShot_) {
            isBombShot_ = true;
        }
        LerpRotateY(0.0f, 0.3f);

    } else if (phaseTimer_ <= kFloorBombWaitTime_) {
        LerpRotateY(PI, 0.3f);
        RotateLR(2.0f);
    } else if (phaseTimer_ <= kFloorAttackEndingLagTime_) {
        //後隙
        LerpRotateZ(0.5f);
    } else {
        isSelectRandomPhase_ = true;
        isReqestOverheadView_ = false;
    }

}
void Enemy::ShockWaveAttack()
{
    if (phaseTimer_ <= kWavePhaseMovePosTime_) {
        isReqestOverheadView_ = true;
        LookTargetNormal(*playerPos_);
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, *target_, 0.05f);

    } else if (phaseTimer_ <= kWaveShotTime_) {

        //yを戻す
        bodyPos_.worldTransform_.translate_.y = Lerp(bodyPos_.worldTransform_.translate_.y, kRadius_, 0.1f);

        if (!isWaveShot_) {
            isWaveShot_ = true;
        }
    } else if (phaseTimer_ < kWavePhaseMaxTime_) {

    } else {
        isSelectRandomPhase_ = true;
        isReqestOverheadView_ = false;
    }


}
void Enemy::RandomFloorChangAttack()
{

    if (phaseTimer_ <= 1.0f) {
        //前隙
        LerpPos({ 0.0f,kRadius_,6.0f }, 0.01f);
        LerpRotateY(PI, 0.3f);

        isGoToRanDomFloor_ = true;

    } else if (phaseTimer_ <= 2.0f) {

        LookTargetNormal(*target_);
        //床に行く
        bodyPos_.worldTransform_.translate_ = Lerp(bodyPos_.worldTransform_.translate_, *target_, 0.01f);

    } else if (phaseTimer_ <= 3.0f) {

        bodyPos_.worldTransform_.rotate_.x = Lerp(bodyPos_.worldTransform_.rotate_.x, HALF_PI, 0.05f);

    } else if (phaseTimer_ <= 4.0f) {

        if (enemyBeak_->isFloorHit_) {
            if (!isFloorBring_) {
                //床を持つ
                isFloorBring_ = true;
            }
        }
        //床から離れる
    } else if (phaseTimer_ <= 5.0f) {

        LerpPos({ 0.0f,kRadius_,6.0f }, 0.1f);
        bodyPos_.worldTransform_.rotate_.x = Lerp(bodyPos_.worldTransform_.rotate_.x, 0.0f, 0.05f);

    } else if (phaseTimer_ <= 6.0f) {
        //タイマーによって強制的に床を消去
        isFloorBring_ = false;
    } else {
        isSelectRandomPhase_ = true;

    }

}
void Enemy::Exit()
{
    LerpPos({ 0.0f,kLerpSquareEndPosY_,6.0f }, 0.05f);
    if (phaseTimer_ <= kExitSpinTime_) {
        SpinBody();
    } else {
        if (phaseTimer_ <= 2.0f) {
            float localTimer = phaseTimer_ - 1.0f;
            bodyPos_.worldTransform_.rotate_.x = Easing::EaseInOutBack(0.0f, -QUARTER_PI * 3.0f, localTimer);
        }
        bodyPos_.worldTransform_.rotate_.z = Lerp(bodyPos_.worldTransform_.rotate_.z, 0.0f, 0.5f);
        bodyPos_.worldTransform_.translate_.y = Lerp(bodyPos_.worldTransform_.translate_.y, 0.5f, 0.05f);
    }
}


void Enemy::UpdatePhaseTimer()
{
    phaseTimer_ += InverseFPS;
}

// =============================//アニメーション//============================================================

void Enemy::LookTargetAndSetPos(Vector3& target)
{
    startPos_ = GetWorldPosition();
    endPos_ = target;
    Vector3 direction = endPos_ - startPos_;
    bodyPos_.worldTransform_.rotate_.y = std::atan2(direction.x, direction.z); // Y軸回転（ラジアン）
}


void Enemy::LookTargetNormal(Vector3& target)
{
    Vector3 direction = target - GetWorldPosition();
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
    Sound::PlayOriginSE(Sound::kStun);

    float localTimer = (phaseTimer_ - 0.125f) / (0.75f - 0.125f);
    float theta = PI * 3.0f * localTimer; // 回転の速さを調整
    bodyPos_.worldTransform_.rotate_.z = sinf(theta);
    bodyPos_.worldTransform_.rotate_.x = cosf(theta);
}

void Enemy::LerpSpinOriginBodyXZ()
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

void Enemy::RotateLR(const float& speed)
{
    rotateLRTheta_ += std::numbers::pi_v<float>*InverseFPS * speed;
    bodyPos_.worldTransform_.rotate_.z = sinf(rotateLRTheta_) * 0.5f;
}

void Enemy::LerpRotateZ(const float& speed)
{
    bodyPos_.worldTransform_.rotate_.z = Lerp(bodyPos_.worldTransform_.rotate_.z, 0.0f, speed);
}

// =============================//フェーズ抽選//============================================================

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
        RandomAttackPhaseFirst();
        break;
    case 1:
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

