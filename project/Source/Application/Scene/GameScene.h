#pragma once

#include"SceneManager.h"

#include<memory>
#include<array>

//Transformなどの情報が入っている
#include"Object3d.h"

#pragma region//ゲームオブジェクト
#include"Player.h"
#include"World.h"
#include"Locker.h"
#include"Field.h"
#include"Enemy.h"
#pragma endregion

class ParticleEmitter;
class ParticleManager;

class GameScene :public SceneManager {

public:
    GameScene();
    ~GameScene() override;
    void Initialize()override;
    void Update()override;
    void Draw()override;
    void Debug()override;

private:
    void UpdateCamera();
    void UpdateGameObject();
    void UpdateParticle();
    //全衝突判定
    void CheckAllCollision();
private:
#pragma region//ゲームオブジェクト
    std::unique_ptr<Player>player_ = nullptr;
    std::unique_ptr<World>world_ = nullptr;
    std::unique_ptr<Field>filed_ = nullptr;
    std::unique_ptr<Enemy>enemy_ = nullptr;
#pragma endregion
    std::unique_ptr<ParticleEmitter>particleEmitter_ = nullptr;
    ParticleManager* particleManager_ = nullptr;
};
