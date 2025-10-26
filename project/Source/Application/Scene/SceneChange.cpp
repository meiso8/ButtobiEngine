#include "SceneChange.h"


void SceneChange::Initialize()
{
    isEndScene_ = false;
    timer_ = 0;

}

void SceneChange::Update(uint32_t endTime)
{
    timer_++;
    if (timer_ > endTime) {
        isEndScene_ = true;
    }
}
