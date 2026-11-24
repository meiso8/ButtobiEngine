#include "Ticket.h"
#include"Object3d.h"
#include"Easing.h"
#include"TimeManager.h"

void Ticket::Init()
{
    aniTimer_ = 0.0f;
    isAnimEnd_ = false;
    isGet_ = false;
    isUsed_ = false;

    isMelt_ = false;

    object_->Initialize();
    object_->SetTemperature(0.5f);

    screenEndSize_ = 0.03125f * 0.75f;

    startPos_ = { 0.0f };
    endPos_ = { 0.0f };
    generateAnimationTimer_ = 0.0f;
}

void Ticket::Update()
{
    if (!isGet_) {
        {
            if (generateAnimationTimer_ < 2.0f) {
                generateAnimationTimer_ += TimeManager::DeltaTime();
                Vector3 position = Easing::EaseOutBack(startPos_, endPos_, TimeManager::GetLocalTimer(generateAnimationTimer_, 0.0f, 2.0f));
                object_->SetTranslate(position);
            }
 
        }
    }

    object_->Update();
}
