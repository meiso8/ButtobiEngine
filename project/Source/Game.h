#pragma once
#include"ButtobiEngine.h"
#include<map>
#include"SceneManager.h"
#include<thread>

class Game:public ButtobiEngine
{
public:
    void Initialize()override;
    void Finalize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
private:
    // ロード用スレッドの関数
    void LoadAssets();

    std::thread loadThread_;
    std::atomic<bool> isLoading_{ true }; // ロード中かどうかを判定するフラグ（スレッドセーフ）
};

