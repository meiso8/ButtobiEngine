#pragma once
#include"MyEngine.h"
#include<map>
#include<unordered_map>
#include"SceneManager.h"
class Game
{
public:
    void Initialize();
    void Finalize();
    void Update();
    void Draw();
    void Debug();
private:
    D3DResourceLeakChecker leakCheck;
    std::unique_ptr<MyEngine> myEngine;
    Vector4 screenColor;
    std::map<const std::string, std::unique_ptr<SceneManager>> scenes;
    SceneManager* currentScene = nullptr;
    std::map<std::string, std::unique_ptr<SceneManager>>::iterator currentIt;

};

