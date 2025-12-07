
#pragma once
#include"Object3d.h"
#include <unordered_map>

class Model;
class Camera;
enum LightMode;

class BetoBeto
{
public:

    BetoBeto();
    void Initialize();
    void Update();
    void Draw(Camera& camera);
private:
    Object3d position_;
    Model* model_ = nullptr;
};