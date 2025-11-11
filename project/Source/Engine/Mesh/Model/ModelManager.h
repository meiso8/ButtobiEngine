#pragma once
#include"ModelData.h"
#include<map>

class Model;

class ModelManager
{
private:
public:
    enum MODEL_HANDLE {
        WORLD,
        PLAYER,
        EFFECT,
        LOCKER,
        PEOPLE,
        MEDJED,
        MUMMY,
        BUILDING,
        MODELS,
    };

    static Model* GetModel(const uint32_t& handle);
    static void LoadAllModel();
    static void Finalize();
private:
    static void LoadModel(const std::string& directoryPath, const std::string& filename, const uint32_t& handle);
private:
    static std::map < const uint32_t, std::unique_ptr< Model> > models_;
};

