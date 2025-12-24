#pragma once
#include "Transform.h"
#include<vector>
#include"Matrix4x4.h"
#include<string>
struct Node {
    QuaternionTransform transform;
    Matrix4x4 localMatrix;
    std::string name;
    std::vector<Node>children;
};