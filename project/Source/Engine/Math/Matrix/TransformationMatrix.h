#pragma once
#include"Matrix4x4.h"

struct TransformationMatrixFor2D
{
    Matrix4x4 WVP;
    Matrix4x4 World;
};


struct TransformationMatrixFor3D
{
    Matrix4x4 WVP;
    Matrix4x4 World;
    Matrix4x4 WorldInverseTranspose;
};

struct TransformationMatrixFor3DSkybox
{
    Matrix4x4 WVP;
};