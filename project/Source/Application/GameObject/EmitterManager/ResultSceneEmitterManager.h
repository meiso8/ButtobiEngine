#pragma once

#include"ParticleEmitter.h"
#include<memory>

class Camera;

class ResultSceneEmitterManager
{
public:
    ResultSceneEmitterManager();
    void Create();
    void Initialize();
    void  Update(Camera& camera);
    void Draw();
    void Debug();
private:
    std::unique_ptr<ParticleEmitter>emitter_ = nullptr;

};

