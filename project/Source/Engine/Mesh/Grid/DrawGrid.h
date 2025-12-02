#pragma once

#include"LineMesh.h"
#include"CubeMesh.h"
#include<array>
#include"Object3d.h"
#include<memory>

class DrawGrid {
public:
    static void Finalize();
    static void Create();
    static void Draw(Camera& camera);
private:
    static std::array<std::unique_ptr<LineMesh>, 102> line_;
    static  std::array <std::unique_ptr<CubeMesh>, 2> cube_;

    static std::array< std::unique_ptr<Object3d>, 102> lineTransforms_;



};
