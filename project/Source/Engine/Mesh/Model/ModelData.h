#pragma once

#include"VertexData.h"
#include"MaterialData.h"
#include"Node.h"
#include<cstdint>
#include<map>
struct VertexWeightData {
    float weight;
    uint32_t vertexIndex;
};

struct JointWeightData {
    Matrix4x4 inverseBindPoseMatrix;
    std::vector<VertexWeightData> vertexWeights;
};

struct ModelData {
    std::map<std::string, JointWeightData> skinClusterData;
    std::vector<VertexData> vertices;
    std::vector<uint32_t> indices;
    MaterialData material;
    std::string filePath;
    Node rootNode;
};

