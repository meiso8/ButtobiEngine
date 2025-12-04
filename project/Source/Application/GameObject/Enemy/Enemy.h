#pragma once
#include"WorldTransform.h"
#include"Object3d.h"
#include"AABB.h"
#include"CharacterState.h"
#include"Circle.h"
#include"Collider.h"
#include<memory>
#include<unordered_map>
#include "Engine/Math/Vector/Vector3.h"
class Model;
class Camera;
enum LightMode;

class Enemy :public Collider
{
public:
    enum Parts {
        BODY,
        WING_L,
        WING_R
    };

    Enemy();
    void Init();
    void Draw(Camera& camera, const LightMode& lightMode);
    void Update();
    std::string GetCurrentState() { return currentState_; };
    //コライダーのオーバーライド
    Vector3 GetWorldPosition()const override;
    void OnCollision(Collider* collder)override;
    //ターゲットの座標をセットする
    void SetTarget(Vector3& target) { target_ = &target; };
    //プレイヤーの座標をセットする
    void SetPlayerPos(Vector3& target) { playerPos_ = &target; };
    //プレイヤーの向きを取得する
    void SetPlayerLookDirPos(Vector3& moveDir) { playerLookDir_ = &moveDir; };
    //敵からプレイヤーに向かうベクトルを得る
    Vector3 GetToPlayer() {
        if (playerPos_ != nullptr) {
            return Normalize(*playerPos_ - bodyPos_.worldTransform_.GetWorldPosition());
        }
        return { 0.0f };
    }

    // 死亡後のアニメーション
	void LeathalMoveUpdate();

    //ファイアボールを打つフラグ
    bool isShot_ = false;
    //ボムを投げる
    bool isBombShot_ = false;
    //波を発生させる
    bool isWaveShot_ = false;
    //床を初期化する
    bool isReqestClearFloor_ = false;
	// フェーズを変えているかどうか
	bool isFaseChange_ = false;
    //体の位置
    Object3d bodyPos_;
    enum PHASE {
        //攻撃
        TACKLE,
        FIREBALL,
        FLOORCHANGEATTACK,
        SHOCKWAVEATTACK,
        //元の位置から移動する
        LERP_ROUND_POS,
        LERP_SQUARE_POS,
        //タックル中に何かに当たったら移行する
        KNOCKBACK,
        PLAYER_HIT_BACK,
        //移動フェーズ
        ROUND,
        SQUARE,
        RANDOM_WALK,
        //終了
        EXIT,
        MAX_PHASE
    };
    PHASE phase_ = PHASE::ROUND;

    //弾と当たったかどうかを得る
    bool IsHit() { return damageStruct_.isHit; }
    bool IsDead() { return damageStruct_.isDead; }
    //HPを得る
    HPs* GetHpsPtr() { return &damageStruct_.hps; }
    bool IsOverKill() { return(overKillCount >= kMaxOverKillCount); }
private:

    // 死亡時のぶっ飛び
    bool isLeathalVec_;



    //メンバ関数ポインタテーブル
    std::unordered_map<PHASE, std::function<void()>> UpdateActions_;


    std::string currentState_ = "First";
    //メンバ関数ポインタテーブル　ランダムアクションに切り替わる
    std::unordered_map<std::string, std::function<void()>> SwitchRandomAttackPhase_;

    //モデル
    std::unordered_map< Parts,Model*> models_;
    //翼の位置
    Object3d wingLPos_;
    //翼の位置
    Object3d wingRPos_;

    //目標地点
    Vector3* target_ = nullptr;
    Vector3* playerPos_ = nullptr;
    Vector3* playerLookDir_ = nullptr;

    //キャラクターの共通でもつ状態
    Damage damageStruct_;

    //速度
    Vector3 velocity_ = { 0.0f };
    //球面座標
    SphericalCoordinate sphericalPos_;
    //始点終点
    Vector3 startPos_ = { 0.0f };
    Vector3 endPos_ = { 0.0f };

    //フェーズタイマー
    float phaseTimer_ = 0.0f;
    float phaseTime_ = 0.0f;
    //ランダムフェーズを呼び出すかどうか
    bool isSelectRandomPhase_ = false;

    //何回同じタイプの行動をしたかを記録
    int actionCount_ = 0;
    //攻撃したかどうか
    bool isAttack_ = false;
    //前回のフェーズで攻撃したか
    bool isPreAttack_ = false;

#pragma region //円移動
    //球面移動に移行する時のスピード
    const float kSphericalLerpSpeed_ = 0.5f;
    //円移動時の半径
    const float kRoundRadius_ = 5.5f;
#pragma endregion

#pragma region //タックル
    //フェイントするかどうか
    bool isFeint_ = false;
    //タックル中にプレイヤーに向くタイム
    const float kTackleLookTime_ = 2.0f;
    //タックル中にぽよぽよするタイム
    const float kTacklePoyoTime_ = kTackleLookTime_;
    //タックル終了インターバル
    const float kTackleInterval_ = kTacklePoyoTime_ + 0.5f;
    //プレイヤーに向けてタックルするタイム
    const float kTackleGoPlayerTime_ = kTackleInterval_ + 0.7f;
    //タックル終了後ちょっと戻るタイム
    const float kTackleBackTime_ = kTackleGoPlayerTime_ + 1.0f;
    //タックル終了後初期地点に戻るタイム
    const float kTackleInitStartTime_ = kTackleGoPlayerTime_ + 1.0f;
    //タックル終了後　後隙
    const float kTackleEndingLagTime_ = kTackleInitStartTime_ + 0.5f;
#pragma endregion

#pragma region//Hit Back
    //当たった後ちょっと戻るタイム
    const float kHitBackTime_ = 0.125f;
    //プレイヤーに当たった後初期地点に戻るタイム
    const float kPlayerHitBackTime_ = 2.0f;
    
    //KnockBack中初期地点に戻るタイム
    const float kKnockBackSpinTime_ = 1.0f;
    //KnockBack最大タイム
    const float kKnockBackMaxTime_ = 3.5f;
#pragma endregion

#pragma region //ファイアボール
    //ファイアボールのクールタイム
    float fireBallCoolTimer_ = 0.0f;
    //ファイアボールのクールタイム
    float kFireBallMaxCoolTime_ = 1.5f;
    //一回転する時間
    const float kFireBallRotateTime_ = 1.0f;
    //ファイアボール最大タイム　ここは後隙を含めた値にする
    const float kFireBallPhaseMaxTime_ = 8.5f;
#pragma endregion

#pragma region //床攻撃
    //攻撃地点移動タイム
    const float kFloorAttackPosMoveTime_ = 1.0f;
    //ボムを投げる時間
    const float kFloorBombShotTime_ = kFloorAttackPosMoveTime_ + 0.5f;
    //ボム爆発まで待つ時間
    const float kFloorBombWaitTime_ = 11.0f;
    //ボム爆発後隙
    const float kFloorAttackEndingLagTime_ = kFloorBombWaitTime_ + 1.0f;
#pragma endregion

#pragma region //波攻撃
    //波攻撃地点まで移動するタイム
    const float kWavePhaseMovePosTime_ = 1.0f;
    //波攻撃地点まで移動するタイム
    const float kWaveShotTime_ = kWavePhaseMovePosTime_ + 0.5f;
    //波攻撃の終了タイム　ここは後隙を含めた値にする
    const float kWavePhaseMaxTime_ = 5.0f;
#pragma endregion
#pragma region //四角移動
    //四角移動の初期地点の移動時間
    const float kLerpSquareInitPosTime_ = 1.0f;
    //四角移動に移行する際のタイム
    const float kLerpSquareTime_ = 1.5f;

    const float kLerpSquareStartPosY_ = 4.0f + kRadius_;
    const float kLerpSquareEndPosY_ = 1.0f + kRadius_ * 2.0f;
    //四角移動する際の速度
    const float kSquareMoveSpeed_ = 0.05f;
    //一つの移動についての間隔
    const float kSquareMoveInterval_ = 2.0f;
    const float kSquareMoveMaxTime_ = 8.0f;
#pragma endregion



#pragma region //最後の撃破アニメーション

    //波攻撃地点まで移動するタイム
    const float kExitSpinTime_ = 1.0f;
    //ひっくり返るタイム
    const float kFlipOverTime_ = kExitSpinTime_ + 1.0f;

#pragma endregion


    //開始時Y軸回転
    float startRotateY_ = 0.0f;
    //終了時Y軸回転
    float endRotateY_ = 0.0f;
    //回転速度Y
    float roundSpeedY = 1.0f;

    //半径　コライダーの大きさ
    const float kRadius_ = 1.5f;
    //サイズ
    const float kSize_ = 1.0f;

    //ぽよぽよアニメタイマー
    float poyoAnimTimer_ = 0.0f;
    const float kPoyoAnimeTime_ = 0.25f;

    //翼を動かすときのもの
    float wingTheta_ = 0.0f;
    // 左右に揺れるときのもの
    float rotateLRTheta_ = 0.0f;

    //オーバーキルカウント
    int overKillCount = 0;
    //オーバーキルカウント最大値
    int kMaxOverKillCount = 150;
private:
    //状態を初期化
    void InitState();
    //状態を変更
    void SwitchState();

#pragma region//フェーズ
    //フェーズ変更
    void SetPhase(PHASE phase);

    //フェーズ切り替え
    void SwitchPhase();

    //移動フェーズ内で抽選
    void RandomMovePhase();
    //攻撃フェーズ内で抽選　第一形態時　
    void RandomAttackPhaseFirst();
    //攻撃フェーズ内で抽選　第二形態時　
    void RandomAttackPhaseSecond();
    //攻撃フェーズ内で抽選　第三形態時　
    void RandomAttackPhaseEnd();
    //フェーズタイマーを更新する
    void UpdatePhaseTimer();
#pragma endregion

#pragma region//移動フェーズ
    //回転移動
    void Round();
    //回転移動から戻す処理
    void LerpRoundPos();

    //四角移動
    void SquareMove();
    //四角移動から戻る処理
    void LerpSquarePos();

    //任意位置に移動
    void RandomWalk();
    //回転ラープをする
    void LerpRotateY(const float& endRadius, const float& lerRotateSpeed);
    //座標をらーぷする
    void LerpPos(const Vector3& endPos, const float& lerpPosSpeed);
#pragma endregion

#pragma region//攻撃
    //ファイアボール
    void Fireball();
    //床攻撃
    void FloorChangeAttack();
    //タックル
    void Tackle();
    //プレイヤーとぶつかった時の処理
    void PlayerHitBack();
    //タックル中に弾とぶつかった時の処理
    void KnockBack();
    //衝撃波攻撃
    void ShockWaveAttack();
#pragma endregion

    //離脱
    void Exit();

#pragma region //アニメーション
    //ターゲットを見る
    void LookTarget(Vector3& target);
    //弾が当たった時のアニメ
    void HitAnimation();
    //ぽよぽよアニメ
    void PoyoPoyo(const float& endTimer);
    //ぽよぽよ終了時にスケールを戻す処理
    void LerpScale();
    //回転Y
    void RotateY(const float& timer);

    //くらくらさせる
    void SpinBody();
    //くらくら終了時に戻す処理
    void LerpSpinOriginBody();

    //羽を動かす
    void Winging(const float& speed);

    void RotateLR(const float& speed);
    void LerpRotateZ(const float& speed);
#pragma endregion
};

