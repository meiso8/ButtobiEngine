#include "ModelManager.h"
//ファイルやディレクトリに関する操作を行うライブラリ
#include <filesystem>
#include <fstream>//ファイルの書いたり読んだりするライブラリ
#include<cassert> //assertも利用するため

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"DirectXCommon.h"
#include"Texture.h"
#include"TextureManager.h"
std::vector<ModelData> ModelManager::modelDatas_;
std::vector<uint32_t> ModelManager::handle_;



ModelData& ModelManager::GetModelData(const uint32_t& handle)
{
    assert(handle < modelDatas_.size());
    return modelDatas_[handle];
}

void ModelManager::LoadAllModel()
{
    handle_.resize(MODELS);

    handle_[PLAYER] = Load("resources/player", "player.obj");

    handle_[HEAD] = Load("resources/head", "head.obj");
    handle_[BODY] = Load("resources/body", "body.obj");
    handle_[LEFTARM] = Load("resources/leftArm", "leftArm.obj");
    handle_[RIGHTARM] = Load("resources/rightArm", "rightArm.obj");
    handle_[LEFTLEG] = Load("resources/leftLeg", "leftLeg.obj");
    handle_[RIGHTLEG] = Load("resources/rightLeg", "rightLeg.obj");

    handle_[ENEMY] = Load("resources/enemy", "enemy.obj");
    handle_[WORLD] = Load("resources/world", "world.obj");
    handle_[PARTICLE] = Load("resources/particle", "particle.obj");

    handle_[STAGE] = Load("resources/stage", "stage.obj");

}

// ========================================================================================================

uint32_t ModelManager::Load(const std::string& directoryPath, const std::string& filename)
{
    LoadModel(directoryPath, filename);
    std::string filePath = directoryPath + "/" + filename;
    return GetTextureIndexByFileName(filePath);
}

void ModelManager::LoadModel(const std::string& directoryPath, const std::string& filename)
{
    //読み込み済みテクスチャを検索
    auto it = std::find_if(
        modelDatas_.begin(),
        modelDatas_.end(),
        [&](ModelData& soundData) {return soundData.filePath == filename; }
    );

    //テクスチャ枚数上限チェック
    assert(modelDatas_.size() < DirectXCommon::kMaxModelCount);

    if (it != modelDatas_.end()) {
        return;
    }

    //テクスチャデータを追加
    modelDatas_.resize(modelDatas_.size() + 1);
    //追加したテクスチャデータの参照を取得する
    ModelData& modelData = modelDatas_.back();

    Assimp::Importer importer;
    std::string filePath = directoryPath + "/" + filename;
    modelData.filePath = filePath;

    const aiScene* scene = importer.ReadFile(filePath.c_str(),
        aiProcess_Triangulate |
        aiProcess_FlipWindingOrder |
        aiProcess_FlipUVs |
        aiProcess_SortByPType);

    assert(scene->HasMeshes());

    for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
        aiMesh* mesh = scene->mMeshes[meshIndex];
        assert(mesh->HasNormals());
        assert(mesh->HasTextureCoords(0));

        for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
            aiFace& face = mesh->mFaces[faceIndex];
            assert(face.mNumIndices == 3);//三角形のみサポート
            for (uint32_t element = 0; element < face.mNumIndices; ++element) {
                uint32_t vertexIndex = face.mIndices[element];
                aiVector3D& position = mesh->mVertices[vertexIndex];
                aiVector3D& normal = mesh->mNormals[vertexIndex];
                aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex];
                VertexData vertex;
                vertex.position = { position.x,position.y,position.z,1.0f };
                vertex.normal = { normal.x,normal.y,normal.z };
                vertex.texcoord = { texcoord.x,texcoord.y };


                vertex.position.x *= -1.0f;
                vertex.normal.x *= -1.0f;
                modelData.vertices.push_back(vertex);
            }

        }


    }

    for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
        aiMaterial* material = scene->mMaterials[materialIndex];

        //MultiMaterialに対応していく場合などは改造が必要である
        if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
            aiString textureFilePath;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
            modelData.material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
        }

    }

    //モデルのテクスチャを読む
    Texture::handle_.push_back(TextureManager::Load(modelData.material.textureFilePath));
    modelData.textureHandle = UINT(Texture::handle_.size() - 1);
    assert(modelData.textureHandle < Texture::handle_.size());

}

uint32_t ModelManager::GetTextureIndexByFileName(const std::string& filePath)
{
    //読み込み済みデータを検索
    auto it = std::find_if(
        modelDatas_.begin(),
        modelDatas_.end(),
        [&](ModelData& modelData) {return modelData.filePath == filePath; }
    );

    if (it != modelDatas_.end()) {
        uint32_t modelIndex = static_cast<uint32_t>(std::distance(modelDatas_.begin(), it));
        return modelIndex;
    }

    assert(0);
    return 0;
}
