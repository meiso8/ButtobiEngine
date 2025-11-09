#pragma once
#include<string>
#include <iostream>

struct MaterialData {
    std::string textureFilePath;
    uint32_t textureSrvIndex;
};

MaterialData LoadMaterialTemplateFile(const std::string& dirextoryPath, const std::string& filename);