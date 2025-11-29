
#pragma once
#include"Object3d.h"
#include <unordered_map>
#include"Collider.h"

class Model;
class Camera;
enum LightMode;

class Tree
{
public:
    enum Parts {
        kTree,
        kLeaves,
    };

    Tree();
    void Initialize();
    void Update();
    void Draw(Camera& camera, const LightMode& lightType);
private:
    std::unordered_map<Parts, Object3d> positions_;
};