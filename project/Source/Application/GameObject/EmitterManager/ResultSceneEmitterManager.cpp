#include "ResultSceneEmitterManager.h"
#include"Camera.h"
#include"DebugUI.h"
ResultSceneEmitterManager::ResultSceneEmitterManager()
{
    //ParticleEmitter::Create();
}

void ResultSceneEmitterManager::Create()
{

    emitter_ = std::make_unique<ParticleEmitter>();
    emitter_->SetName("white1x1Particle");

    emitter_->emitter_.count = 1;
    emitter_->emitter_.movement = ParticleMovements::kParticleNormal;
    emitter_->emitter_.transform.translate_.y = 5.0f;
    emitter_->emitter_.translateAABB_ = { .min = {-6.0f,-3.0f,15.0f},.max = {10.0f,2.0f,20.0f} };
    emitter_->emitter_.rotateOffset_ = 0.0f;
    emitter_->emitter_.frequency = 0.1f;
    emitter_->emitter_.lifeTime = 6.0f;
    emitter_->emitter_.blendMode = kBlendModeScreen;
    emitter_->emitter_.transform.scale_ = { 0.2f,0.2f,0.2f };
    emitter_->emitter_.scaleOffset_ = 0.15f;
    emitter_->emitter_.rotateOffset_ = 3.14f;
    emitter_->emitter_.velocityAABB = { .min = {-1.0f,-1.0f,-2.5f} ,.max = {-1.0f,1.0f,-1.0f} };
    emitter_->emitter_.startAlpha_ = 1.0f;
    emitter_->emitter_.endAlpha_ = 0.0f;
    //初期値をこれにするとランダムにするように一旦した。
    emitter_->emitter_.color = { 0.0f,0.0f,0.0f,0.0f };

    //マネージャーから加速度の数値をもらう
    auto& group = emitter_->GetGroup();
    group->accelerationField.area = { .min = {-20.0f,-10.0f,-16.0f},.max = {20.0f,10.0f,16.0f} };
    group->accelerationField.acceleration = { 0.0f, -0.2f,-0.5f };


}

void ResultSceneEmitterManager::Initialize()
{
    emitter_->InitTimer();
    ParticleManager::ResetAll();
}

void ResultSceneEmitterManager::Update(Camera& camera)
{
    //葉っぱ
    emitter_->UpdateTimer();
    emitter_->UpdateEmitter();

    ParticleEmitter::Update(camera);
}

void ResultSceneEmitterManager::Draw()
{
    ParticleEmitter::Draw();
}

void ResultSceneEmitterManager::Debug()
{

    //ここでデバック
    DebugUI::CheckParticle(*emitter_, "emitter_");
}
