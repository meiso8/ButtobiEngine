#pragma once
#include"Vector4.h"
#include"Vector3.h"
#include"hlslTypeToCpp.h"

struct DirectionalLight
{
    Vector4 color;//ライトの色
    Vector3 direction;//ライトの向き　正規化すること
    float intensity;//輝度
};

struct PointLight {
    float32_t4 color;
    float32_t3 position;
    float intensity;
};