#pragma once

#include"LineMesh.h"
#include"Cube.h"
#include<array>

class DrawGrid {
public:
    static void Finalize();
    static void Initialize();
    static void Draw(Camera& camera, bool isDraw = true);
private:
    static std::array<LineMesh*, 102> line_;
    static  std::array < Cube*, 2> cube_;
};
