#pragma once
#include"Vector3.h"
#include <array>
#include <memory>
#include <string>

//無限遠平面
struct Plane {
    Vector3 normal;//法線
    float distance;//距離
};

