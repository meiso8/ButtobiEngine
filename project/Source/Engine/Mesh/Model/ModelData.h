#pragma once
#include<vector>
#include"VertexData.h"
#include"MaterialData.h"

struct Node {
    Matrix4x4 localMatrix;
    std::string name;
    std::vector<Node>children;
};

struct ModelData {
    std::vector<VertexData> vertices;
    MaterialData material;
    std::string filePath;
    Node rootNode;
};

