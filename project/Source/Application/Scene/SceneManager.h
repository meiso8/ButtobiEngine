#pragma once
class SceneManager
{
public:
    SceneManager() = default;
    ~SceneManager() = default;
    virtual void Initialize();
    virtual void Update();
    virtual void Draw();
    virtual void Debug();
};