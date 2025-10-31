#pragma once
#include"Vector4.h"
#include"Matrix4x4.h"
#include<cstdint>
#include"hlslTypeToCpp.h"
//
//struct Material {
//    Vector4 color;
//    int32_t lightType;
//    float padding[3];
//    Matrix4x4 uvTransform;
//};

struct Material
{
    float32_t4 color;
    int32_t lightType;
    float padding[3];
    float32_t4x4 uvTransform;
    float32_t shininess;
    float padding2[3];
};
