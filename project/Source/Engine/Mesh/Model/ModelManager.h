#pragma once
#include"ModelData.h"
#include<map>

class Model;
struct aiNode;
class ModelManager
{
private:
public:
    /// @brief モデルのハンドルを宣言します
    enum MODEL_HANDLE {
        BOX,
      normalMedjed_GLTF,
        medJed_GLTF,
        //プレイヤーそれぞれのパーツ
        PLAYER_BODY,

        PEOPLE,
        MUMMY,
        LOCKER,
        BUILDING,


        KEY,

        KEY_A,
        KEY_D,
        KEY_ESC,
        KEY_S,
        KEY_SPACE,
        KEY_W,

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
    static Node ReadNode(aiNode* node);
private:
    static std::map < const uint32_t, std::unique_ptr< Model> > models_;
};

