#include "FrameStopManager.h"

void FrameStopManager::Update()
{
	if (isFrameStop_) {
		nowFrame_++;
		if (nowFrame_ >= stopFrame_) {
			isFrameStop_ = false;
			nowFrame_ = 0;
			stopFrame_ = 0;
		}
	}
}

void FrameStopManager::StopFrame(int stopFrame)
{
	isFrameStop_ = true;
	stopFrame_ = stopFrame;
	nowFrame_ = 0;
}
