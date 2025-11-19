#pragma once
#include"ModelData.h"
#include<map>

class Model;

class ModelManager
{
private:
public:
    /// @brief モデルのハンドルを宣言します
    enum MODEL_HANDLE {
        BOX,

        //プレイヤーそれぞれのパーツ
        PLAYER_BODY,

        MEDJED,
        PEOPLE,
        MUMMY,
        LOCKER,
        BUILDING,
        WORLD,
        MODELS,
    };
    /// @brief モデルを取得します
    /// @param handle モデルハンドル
    /// @return Modelのポインタ
    static Model* GetModel(const uint32_t& handle);
    /// @brief 全てのモデルをロードします
    static void LoadAllModel();

    static void Finalize();
private:
    static void LoadModel(const std::string& directoryPath, const std::string& filename, const uint32_t& handle);
private:
    static std::map < const uint32_t, std::unique_ptr< Model> > models_;
};

