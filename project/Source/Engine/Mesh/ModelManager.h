#pragma once
#include"ModelData.h"

class ModelManager
{
private:
public:
    enum MODEL_HANDLE {
        PLAYER,
        MODELS,
    };

    static ModelData& GetModelData(const uint32_t& handle);
    static void LoadAllModel();

private:

    static void LoadModel(const std::string& directoryPath, const std::string& filename);
    //インデックスの開始番号
    static uint32_t Load(const std::string& directoryPath, const std::string& filename);
    static uint32_t GetTextureIndexByFileName(const std::string& filename);
private:
    static std::vector<ModelData> modelDatas_;
    static std::vector<uint32_t> handle_;
};

