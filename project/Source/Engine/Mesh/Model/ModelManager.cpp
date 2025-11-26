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
#include"Model.h"

std::map<const uint32_t, std::unique_ptr< Model> >ModelManager::models_;

void ModelManager::LoadAllModel()
{
    //モデルのファイルパスとタグを関連付けてください

    LoadModel("Resource/Models/Box", "Box.obj", BOX);
    LoadModel("Resource/Models/player", "player.obj", PLAYER_BODY);
    LoadModel("Resource/Models/medjed", "medjed.obj", MEDJED);
    LoadModel("Resource/Models/people", "people.obj", PEOPLE);
    LoadModel("Resource/Models/mummy", "mummy.obj", MUMMY);
    LoadModel("Resource/Models/locker", "locker.obj", LOCKER);
    LoadModel("Resource/Models/building", "building.obj", BUILDING);
    LoadModel("Resource/Models/world", "world.obj", WORLD);

}

// ========================================================================================================

Model* ModelManager::GetModel(const uint32_t& handle)
{
    assert(handle < models_.size());

    if (models_.contains(handle)) {
        return models_.at(handle).get();
    }
    return nullptr;

}
void ModelManager::Finalize()
{
    models_.clear();
}

// ========================================================================================================

void ModelManager::LoadModel(const std::string& directoryPath, const std::string& filename, const uint32_t& handle)
{
    //読み込み済みテクスチャを検索
    if (models_.contains(handle)) {
        return;
    }
    //テクスチャ枚数上限チェック
    assert(models_.size() < DirectXCommon::kMaxModelCount);

    //追加したテクスチャデータのポインタ
    std::unique_ptr<Model> model = std::make_unique<Model>();
   
    std::unique_ptr<ModelData> modelData = std::make_unique<ModelData>();

    Assimp::Importer importer;
    std::string filePath = directoryPath + "/" + filename;
    modelData->filePath = filePath;

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
                modelData->vertices.push_back(vertex);
            }

        }


    }

    for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; ++materialIndex) {
        aiMaterial* material = scene->mMaterials[materialIndex];

        //MultiMaterialに対応していく場合などは改造が必要である
        if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
            aiString textureFilePath;
            material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);
            modelData->material.textureFilePath = directoryPath + "/" + textureFilePath.C_Str();
        }

    }

    //モデルのテクスチャを読む
    modelData->material.textureSrvIndex = Texture::AddTextureHandle(modelData->material.textureFilePath);
    model->SetModelData(std::move(modelData));
    model->Create();

    //ハンドルとモデルをセットにする
    models_.insert(std::make_pair(handle, std::move(model)));

}
