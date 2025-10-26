#include "SceneChange.h"


void SceneChange::Initialize()
{
    isEndScene_ = false;
    isSceneStart_ = false;
    endTimer_ = 0;
    startTimer_ = 0;
}


void SceneChange::UpdateStart(uint32_t endTime)
{
    startTimer_++;
    if (startTimer_ > endTime) {
        //シーンが始まるよ！
        isSceneStart_ = true;
    }
}

void SceneChange::UpdateEnd(uint32_t endTime)
{
    endTimer_++;
    if (endTimer_ > endTime) {
        isEndScene_ = true;
    }
}
