#include "MummyStage.h"

#include"SoundManager/SoundManager.h"
#include"InputBind.h"
#include<algorithm>
#include"TimeManager.h"
#include"Sound.h"
#include"RaySprite/RaySprite.h"
#include "ParticleEmitter.h"
#include"DebugUI.h"

#include"FlagManager/FlagManager.h"
MummyStage::MummyStage()
{
    papyrus_ = std::make_unique<Papyrus>();
    mummy_ = std::make_unique<Mummy>();
    mummyRoom_ = std::make_unique<MummyRoom>();

    for (auto& dummyMummy : dummyMummies_) {
        dummyMummy = std::make_unique<DummyMummy>();
    }

    DummyMummy::SetTargetPosPtr(&GetPlayer()->GetBodyWorldTransform().eTransform_.translate);

    CreateParticle();
}

MummyStage::~MummyStage()
{
}


void MummyStage::Initialize() {


    mummyRoom_->Init();
    mummyRoom_->Update();

    papyrus_->Initialize();
    mummy_->Initialize();



    const float distance = 7.0f;
    const float spacing = 2.5f;

    for (int i = 0; i < kMaxMummy_; ++i) {
        dummyMummies_[i]->Initialize();

        Vector3 pos;
        float rotate;

        if (i < 5) {
            // 北壁
            pos = { -spacing * 2.0f + i * spacing, 0.0f, -distance };
            rotate = 0.0f;
        } else if (i < 9) {
            // 東壁
            pos = { distance, 0.0f, -distance + (i - 5) * spacing + spacing };
            rotate = -1.57f;
        } else if (i < 13) {
            // 西壁

            pos = { -distance, 0.0f, -distance + (i - 9) * spacing + spacing };
            rotate = 1.57f;
        }

        dummyMummies_[i]->SetPosition(pos);
        dummyMummies_[i]->SetRotateY(rotate);
        dummyMummies_[i]->Update();
    }

    //メジェド様は一回現れるのさ
    isShowMedjed_ = false;

}

void MummyStage::StageTransitionInitialize()
{

    Sound::PlaySE(SoundFactory::HORROR1);

    //ステージのロード
    LoadAndCreateObject("MummyStage_objectEditor");

    GetMemoManager()->GenerateMemos({ TextureFactory::BOOK,TextureFactory::MEMO5 });
    // ミイラ前に移動
    GetPlayer()->Init({ 0.0f, 0.0f, -5.0f });

    GetItemManager()->GenerateItems({});

}

void MummyStage::Update() {


    auto item = GetItemManager()->GetItem("GoldHeart");

    if (GetItemManager() && item && item->IsUsed() && item->IsGetAnimEnd()) {
        //メジェドあらわる
        if (!isShowMedjed_) {
            Sound::PlaySE(SoundFactory::BELL);
            isShowMedjed_ = true;
            //壁の当たり判定に戻す
            SetMummyTypeWall();
            FlagManager::SetFlag("ShowMedjed");
        }

    };

    if (isShowMedjed_) {
        //メジェド様が現れたらパーティクルを出す
        particleEmitter_->Update();
        //メジェドが現れてメジェドステージに行ったらメジェドステージに行けるフラグを真にする
        FlagManager::SetFlag("GoMedjedStage");

    } else {
        //メジェド様が現れてないときミイラが動き出す

        if (mummy_->GetIsOpen()) {
            //棺が空いたら
            for (auto& dummyMummy : dummyMummies_) {
                if (!dummyMummy->GetIsOpen()) {
                    dummyMummy->SetIsOpen(true);
                    dummyMummy->SetCollisionType(true);
                }
            }
        }

        for (auto& dummyMummy : dummyMummies_) {
            dummyMummy->Update();
        }
    }

    papyrus_->Update();
    mummy_->Update();
    mummyRoom_->Update();

#ifdef USE_IMGUI

    DebugUI::CheckEmitter(particleEmitter_->GetEmitter());

#endif // !USE_IMGUI

}

bool MummyStage::IsRayCastHit(RaySprite& raySprite)
{

    if (raySprite.Intersect(mummy_.get())) {

        if (InputBind::IsClick()) {
            //Openしていなかったらmummyをあける
            if (!mummy_->GetIsOpen()) {
                SoundManager::PlayCorrectSE();
                mummy_->SetIsOpen(true);
            }
        }


        return true;
    }

    return false;
}

void MummyStage::CheckCollision(CollisionManager& collisionManager, RaySprite& raySprite)
{

    if (IsRayCastHit(raySprite)) {

        //オープンし終わったら
        if (mummy_->GetIsOpenEnd()) {
            const Vector3 mummyPos = mummy_->GetWorldTransform().GetWorldPosition();
            Vector3 endPosOffset_ = { -0.3f,0.3f,0.01f };
            Vector3 startPosOffset_ = { -0.3f,0.5f,0.01f };

            //心臓を使う
            GetItemManager()->UseItemFromSlot(mummyPos + startPosOffset_, mummyPos + endPosOffset_, "GoldHeart"
            );
        }

    }


    for (auto& dummyMummy : dummyMummies_) {
        if (!dummyMummy->GetIsHitCollision())
            collisionManager.AddCollider(dummyMummy.get());
    }

    //ミイラの台も一緒に
    collisionManager.AddCollider(mummy_.get());
    collisionManager.AddCollider(mummy_->GetPlatform());
    collisionManager.AddCollider(papyrus_.get());

    //Waterのかべ
    for (auto& [type, object] : mummyRoom_.get()->GetFieldPoses()) {
        collisionManager.AddCollider(object.get());
    }

}

void MummyStage::Draw() {

    mummyRoom_->Draw();
    papyrus_->Draw();

    for (auto& dummyMummy : dummyMummies_) {
        dummyMummy->Draw();
    }

    mummy_->Draw();


}

void MummyStage::CreateParticle()
{

    particleEmitter_ = std::make_unique<ParticleEmitter>();
    particleEmitter_->Initialize();

    particleEmitter_->SetName("medjedParticle");
    auto& emitter0 = particleEmitter_->GetEmitter();
    emitter0.count = 8;
    emitter0.startColor = { 1.0f,0.75f,0.75f,1.0f };
    emitter0.endColor = { 1.0f,0.75f,0.75f,0.0f };
    emitter0.frequencyTime = 0.25f;
    emitter0.lifeTime = 6.0f;
    emitter0.blendMode = kBlendModeMultiply;
    emitter0.movement = ParticleMovements::kParticleSphere;
    emitter0.radius = 3.0f;
    float pi = std::numbers::pi_v<float>;
    emitter0.rotateAABB_ = { .min = {-pi ,-pi ,-pi } ,.max = { pi, pi, pi} };
    emitter0.isLoop_ = true;
    emitter0.accelerationField_.acceleration.y = 5.0f;
    emitter0.accelerationField_.area = { .min = {-25.0f,0.0f,-25.0f},.max = {25.0f,40.0f,4.0f} };
}

void MummyStage::SetMummyTypeWall()
{      
    //棺が空いたら
    for (auto& dummyMummy : dummyMummies_) {
        dummyMummy->SetCollisionType(false);
    }
}
