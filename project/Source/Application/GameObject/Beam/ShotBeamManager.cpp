#include "ShotBeamManager.h"

#include"Sound.h"
#include"CollisionManager.h"
#include"InputBind.h"

#include"MakeMatrix.h"
#include"TimeManager.h"
#include"DebugUI.h"
#include"RaySprite/RaySprite.h"
#include "BeamManager.h"

namespace {
    constexpr float kInterval_ = 2.0f;

}

ShotBeamManager::ShotBeamManager(const Enemy* enemy, Player* player)
    :enemy_(enemy), player_(player)
{
    beamManager_ = std::make_unique<BeamManager>();
    CreateParticleEmitter();
}

void ShotBeamManager::Initialize()
{

    beamManager_->Initialize();
    currentTime_ = kInterval_;
}

void ShotBeamManager::Update()
{
    
    beamManager_->Update();

#ifdef USE_IMGUI
    DebugUI::CheckEmitter(shockEmitter_->GetEmitter(), "shockEmitter");
#endif // !USE_IMGUI
    auto& emitter0 = beamParticleEmitters_[0]->GetEmitter();
    auto& emitter1 = beamParticleEmitters_[1]->GetEmitter();
    auto& emitter3 = shockEmitter_->GetEmitter();
    if (enemy_->GetPhase() != Enemy::BEAM) {
        currentTime_ = 0.0f;

        emitter0.frequencyTime = 0.0f;
        emitter1.frequencyTime = 0.0f;
        emitter3.frequencyTime = 0.0f;
        return;
    }

    //shockEmitter_->Update();

    if (enemy_->GetIsShotStart()) {

        currentTime_ -= TimeManager::DeltaTime();

        //目のマトリックスを得る
         const Matrix4x4* enemyEyeMatL = &enemy_->GetEyeMats().at("eye_L");
         const Matrix4x4* enemyEyeMatR = &enemy_->GetEyeMats().at("eye_R");

         //目から発射する
        emitter0.transform.eTransform_.translate = Math::GetWorldTransformByMatrix(*enemyEyeMatL);
        emitter1.transform.eTransform_.translate = Math::GetWorldTransformByMatrix(*enemyEyeMatR);

        for (auto& emitter : beamParticleEmitters_) {
            emitter->Update();
        }

        if (currentTime_ <= 0.0f) {
            
            Vector3 target = player_->GetBodyWorldTransform().GetWorldPosition();

            if (beamManager_->ShotBeam(target, enemyEyeMatL, Beam::kEnemy) && beamManager_->ShotBeam(target, enemyEyeMatR, Beam::kEnemy)) {
                Sound::PlaySE(SoundFactory::BEAM);
                Initialize();
            }

        }
    }


}
bool ShotBeamManager::IntersectsAABB(const Ray& ray, const AABB& aabb, const Vector3& pos, const float kMaxDistance)
{
    if (RayIntersectsAABB(ray, aabb, tMin_, tMax_)) {
        float dist = Distance(ray.origin, pos);
        if (dist <= kMaxDistance) {
            return true;
        }
    }
    return false;
}
void ShotBeamManager::RayCastHit(RaySprite& raySprite)
{
    float min = 0.0f;

    //中心点を考慮した座標を取得してくる
    const Vector3 pos = player_->GetEyeWorldPosition();

    AABB aabbWorld = { .min = {-0.25f,-0.25f,-0.25f},.max = {0.25f,0.25f,0.25f} };
    aabbWorld.min += pos;
    aabbWorld.max += pos;

    for (auto& beam : beamManager_->GetBeams()) {

        if (!beam->GetIsActive()) { continue; }

        Ray ray = beam->GetRay();
        float length = Length(ray.diff);

        //アイテムがあれば　跳ね返し攻撃が出来るように作成していく予定

        if (IntersectsAABB(
            ray,
            aabbWorld,
            player_->GetEyeWorldPosition(),
            length
        )) {

            raySprite.OnCollisionColor();

            if (InputBind::IsClick()) {


                if (beam->GetBeamType() != Beam::kPlayer) {

                    Sound::PlaySE(SoundFactory::FALL, 1.5f);
                    Ray ray = raySprite.GetRay();
                    //rayのオリジンから　rayの方向にLength分shotする
                    Vector3 target = ray.origin + ray.diff * length;
                    //親なし
                    beam->Shot(target, Beam::kPlayer, ray.origin, nullptr);
                }

            } else {

                player_->OnCollisionEnemy();
            }

        }
    }
}

void ShotBeamManager::Draw()
{
    beamManager_->Draw();
}

void ShotBeamManager::CreateParticleEmitter()
{

    for (auto& emitter : beamParticleEmitters_) {
        emitter = std::make_unique<ParticleEmitter>();
        emitter->Initialize();
        emitter->SetName("particle1");
    }

    auto& emitter0 = beamParticleEmitters_[0]->GetEmitter();

    emitter0.count = 8;
    emitter0.startColor = { 1.0f,1.0f,1.0f,1.0f };
    emitter0.endColor = { 1.0f,1.0f,1.0f,0.0f };
    emitter0.transform.eTransform_.scale = { 0.025f,0.5f,0.5f };
    emitter0.transform.eTransform_.rotate = { 0.0f,0.0f,0.0f };
    emitter0.transform.eTransform_.translate = { 0.0f,0.0f,0.0f };

    emitter0.frequencyTime = 0.01f;
    emitter0.frequency = 0.1f;
    emitter0.lifeTime = 0.1f;
    emitter0.blendMode = kBlendModeAdd;
    emitter0.movement = ParticleMovements::kParticleNormal;
    emitter0.rotateAABB_ = { .min = {0.0f,0.0f,-3.14f},.max = {0.0f,0.0f,3.14f} };
    emitter0.scaleAABB_ = { .min = {0.0f,0.4f,0.0f},.max = {0.0f,1.5f,1.0f} };
    emitter0.isLoop_ = true;

    auto& emitter1 = beamParticleEmitters_[1]->GetEmitter();
    emitter1 = emitter0;

    emitter1.accelerationField_.area = { .min = {-1.0f,-1.0f,-1.0f},.max = {1.0f,1.0f,1.0f} };
    emitter1.useBillboard_ = true;

    shockEmitter_ = std::make_unique<ParticleEmitter>();
    shockEmitter_->SetName("shockParticle");
    shockEmitter_->Initialize();

    auto& emitter3 = shockEmitter_->GetEmitter();

    emitter3.isLoop_ = true;
    emitter3.useRadialEmission_ = true;
    emitter3.transform.eTransform_.scale = { 0.1f,0.2f,0.1f };
    emitter3.transform.eTransform_.translate = { 0.0f,0.2f,0.0f };
    emitter3.radius = 5.0f;
    emitter3.radiusSpeed = 0.2f;
    emitter3.count = 20;
    emitter3.lifeTime = 1.0f;
    emitter3.movement = ParticleMovements::kParticleSphere;
    emitter3.polarSpeed = 0.0f;
    emitter3.transform.Parent(enemy_->GetWorldTransform());
    emitter3.useBillboard_ = false;
}

