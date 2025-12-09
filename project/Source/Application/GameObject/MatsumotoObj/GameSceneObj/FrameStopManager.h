#pragma once
class FrameStopManager
{
public:
	static FrameStopManager& GetInstance() {
		static FrameStopManager instance;
		return instance;
	}

	void Update();
	void StopFrame(int stopFrame);

	bool isFrameStop_;
private:
	FrameStopManager() = default;
	~FrameStopManager() = default;
	int stopFrame_;
	int nowFrame_;
};