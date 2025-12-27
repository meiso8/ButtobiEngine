#include "ItemSlot.h"
#include"DebugUI.h"
#include"Sound.h"
#include"SoundManager/SoundManager.h"
#include"SpriteCamera.h"
#include"Model.h"
#include"Input.h"
using namespace std;
#define rep(i,n)for(int i =0;i < n;++i)
#include"CollisionConfig.h"
Item::Item()
{
    object_ = std::make_shared<Object3d>();
    object_->Create();
    SetType(kAABB);
    SetAABB({ .min = { -0.5f,-0.5f,-0.5f},.max = { 0.5f,0.5f,0.5f } });

    SetCollisionAttribute(kCollisionItem);

   SetCollisionMask(!kCollisionItem);
}
void Item::SetModel(const ModelManager::MODEL_HANDLE& handle)
{
    object_->SetMesh(ModelManager::GetModel(handle));

}
void Item::Init()
{
    object_->Initialize();
}
void Item::DrawInfoUI()
{
#ifdef USE_IMGUI

    ImGui::Begin("Item");
    //ImGui::Text(description_.c_str());
    DebugUI::CheckObject3d(*object_, name_.c_str());

    ImGui::End();

#endif
}

void Item::Draw(Camera& camera)
{
    object_->Draw(camera);
    //ColliderDraw(camera);
}

void Item::OnCollision(Collider* collider)
{
    object_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
    OnCollisionCollider();
}

void Item::Update()
{
    object_->Update();
    object_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
    ColliderUpdate();
}


ItemSlot::ItemSlot()
{
    rep(i, slotSprites_.size()) {
        slotSprites_[i] = make_unique<Sprite>();
        slotSprites_[i]->Create(Texture::HART, { 0.0f,0.0f });
    }
}

void ItemSlot::OnTriggerItemPickup(const std::shared_ptr<Item>& item)
{
    if (!AddItem(item)) {
        SoundManager::PlayCorrectSE();
    } else {
        SoundManager::PlayCancelSE();
    }
}

void ItemSlot::Update()
{

    for (auto& slot : slots_) {
    }
    Vector2 pos = Input::GetCursorPosition();

    for (auto& sprite : slotSprites_) {
        if (IsCollision(pos, *sprite)) {
            sprite->SetColor({ 1.0f,0.0f,0.0f,1.0f });
        } else {
            sprite->SetColor({ 1.0f,1.0f,1.0f,1.0f });
        }

    }
}

bool ItemSlot::AddItem(const std::shared_ptr<Item>& item)
{
    for (auto& slot : slots_) {
        if (!slot) {
            slot = item;
            return true;
        }
    }
    return false; // 空きスロットがなかった
}

void ItemSlot::UseItem(int index)
{
    slots_[index]->Use();
}

void ItemSlot::CombineItems(int indexA, int indexB)
{
    if (!slots_[indexA] || !slots_[indexB]) return;
    auto combined = slots_[indexA]->CombineWith(slots_[indexB]);

    if (!combined) { combined = slots_[indexB]->CombineWith(slots_[indexA]); }

    if (combined) {
        slots_[indexA] = combined;
        slots_[indexB].reset();
        //ShowMessage("アイテムを組み合わせて「" + combined->name_ + "」を作成した！");
        SoundManager::PlayCorrectSE();
    } else {
        SoundManager::PlayCancelSE();
    }
}

void ItemSlot::DrawInfoUI()
{
    for (auto& item : slots_) {
        if (item) {
            item->DrawInfoUI();
        }
    }

}

void ItemSlot::Draw(Camera& camera)
{
    for (auto& item : slots_) {
        if (item) {
            item->Draw(camera);
        }

    }

}

