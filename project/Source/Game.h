#pragma once
#include"MyEngine.h"
#include<map>
#include<unordered_map>
#include"SceneManager.h"
class Game:public MyEngine
{
public:
    void Initialize()override;
    void Finalize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;
private:
    std::map<const std::string, std::unique_ptr<BaseScene>> scenes;
    SceneManager* currentScene = nullptr;
    std::map<std::string, std::unique_ptr<SceneManager>>::iterator currentIt;

};

