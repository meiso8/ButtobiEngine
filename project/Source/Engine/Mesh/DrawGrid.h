#pragma once

#include"LineMesh.h"
#include"Cube.h"

class DrawGrid {
public:
    static void Initialize();
    static void Draw(Camera& camera);
private:
    static LineMesh line_[102];
    static Cube cube_[2];
};
