#pragma once

#include"LineMesh.h"
#include"Cube.h"
#include<array>
#include"Object3d.h"

class DrawGrid {
public:
    static void Finalize();
    static void Create();
    static void Draw(Camera& camera);
private:
    static std::array<LineMesh*, 102> line_;
    static  std::array <Cube*, 2> cube_;

    static std::array<Object3d*, 104> lineTransforms_;



};
