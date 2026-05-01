#pragma once
#include"ModelData.h"
#include<map>

class Model;
struct aiNode;
class ModelManager
{
private:
public:
    /// @brief モデルを取得します
    /// @param handle モデルハンドル
    /// @return Modelのポインタ
    static Model* GetModel(const std::string& tag);
    static void Finalize();
    static void LoadModel(const std::string& directoryPath, const std::string& filename);
private:
    static Node ReadNode(aiNode* node);
private:
    static std::map < const std::string, std::unique_ptr< Model> > models_;
};

