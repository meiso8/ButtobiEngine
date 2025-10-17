#pragma once
#include"ModelData.h"

class ModelManager
{
private:
public:
    enum MODEL_HANDLE {
        PLAYER,

        HEAD,
        BODY,
        LEFTARM,
        RIGHTARM,
        LEFTLEG,
        RIGHTLEG,
      
        WORLD,
      
        PARTICLE,

        STAGE,

        TITLE_BU, //ぶ
        TITLE_TT, // っ
        TITLE_TO, // と
        TITLE_BI, // び
        TITLE_MI, // み
        TITLE_KK, // っ
        TITLE_KU, // く
        TITLE_SU, // す

        JUICE_CUP,

        TABLE,
      
        FRUIT_APPLE,//リンゴ

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

