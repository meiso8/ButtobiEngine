#pragma once
#include<vector>
#include"VertexData.h"
#include"MaterialData.h"

struct ModelData {
    std::vector<VertexData> vertices;
    MaterialData material;
    std::string filePath;
    uint32_t textureHandle;
};