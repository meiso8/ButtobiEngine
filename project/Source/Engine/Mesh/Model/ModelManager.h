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

        ENEMY_BODY,
        ENEMY_WING_L,
        ENEMY_WING_R,

        FLOOR,
        MELT_FLOOR,

        HOUSE_FLOOR,
        HOUSE_WALL_L,
        HOUSE_WALL_R,
        HOUSE_WALL_BACK,
        HOUSE_DOOR,

        TREE,

        //プレイヤーそれぞれのパーツ
        PLAYER_HEAD,
        PLAYER_ARM_L,
        PLAYER_ARM_R,
        PLAYER_BODY,
        PLAYER_LEG_L,
        PLAYER_LEG_R,

        PLAYER_MODEL,//プレイヤーの全体

		HEAL_ITEM,
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

