#pragma once

#include"VertexData.h"
#include"MaterialData.h"
#include"Node.h"

struct ModelData {
    std::vector<VertexData> vertices;
    MaterialData material;
    std::string filePath;
    Node rootNode;
};

