#pragma once

#pragma region //自作関数
#include"D3DResourceLeakChecker.h"

#include"Input.h"
#include"DirectXCommon.h"
#include"SRVmanager/SrvManager.h"

#include"PSO.h"

#include"ModelManager.h"

#include"Sound.h"

#include"CrashHandler.h"
#include"Log.h"
#include"DebugUI.h"


#include<memory>

#pragma endregion

class MyEngine {
public:
    MyEngine() = default;
    MyEngine(MyEngine&) = delete;
    MyEngine& operator=(MyEngine&) = delete;

    void Create(const std::wstring& title, const int32_t clientWidth, const int32_t clientHeight);
    void Update();
    void Debug();
    void PreCommandSet(Vector4& screenColor);
    void PostCommandSet();
    void Finalize();

private:
    static std::unique_ptr<DirectXCommon> directXCommon;
#ifdef USE_IMGUI
    ImGuiClass imGuiClass = {};
#endif // USE_IMGUI

    static std::unique_ptr<LogFile> logFile;
    static std::unique_ptr<Window> wc;
   static std::unique_ptr <Input> input;

    static std::unique_ptr<PSO> pso;
    static std::unique_ptr<SrvManager> srvManager;
    static std::unique_ptr<ParticleManager> particleManager_;
};

