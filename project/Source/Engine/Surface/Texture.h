#pragma once

#include <fstream>
//ComPtr(コムポインタ)
#include<wrl.h>
#include<d3d12.h>

//Textureデータを読み込むためにDirectXTex.hをインクルード
#include"../externals/DirectXTex/DirectXTex.h"
//Textureの転送のために
#include"../externals/DirectXTex/d3dx12.h"

#include"CommandList.h"
#include<unordered_map>

class Texture
{

public:
    enum TEXTURE_HANDLE {
        WHITE_1X1,
        UV_CHECKER,
        NUMBERS,

        TEXTURES
    };
    static void LoadAllTexture();

    static uint32_t AddTextureHandle(const std::string& filePath);

    static uint32_t GetHandle(const uint32_t& handle) { return handles[handle]; }

    //SRVインデックスの開始番号
    static uint32_t kSRVIndexTop;
    struct TextureData {
        std::string filePath;
        DirectX::TexMetadata metadata;
        Microsoft::WRL::ComPtr<ID3D12Resource> resource;
        Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
        uint32_t srvIndex;
        D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
        D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
    };
public:
    static void Finalize();
    //初期化
    static void Initialize();
//インデックスを返すロード関数
    static uint32_t Load(const std::string& filePath);
    //SRVインデックスの開始番号
    static uint32_t GetSrvIndexByFilePath(const std::string& filePath);

    //テクスチャ番号からGPUハンドルを取得
    static D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU(const std::string& filePath);
    static const DirectX::TexMetadata& GetMetaData(const uint32_t& handle);
private:
    static std::unordered_map<std::string,TextureData> textureDatas;
    static std::vector<uint32_t> handles;
    static std::unordered_map<uint32_t, std::string> handleToPath_;
private:
    //コンストラク・タデストラクタの隠ぺい
    Texture() = default;
    ~Texture() = default;
    //コピーコンストラクタの封印
    Texture(Texture&) = delete;
    //コピー代入演算子の封印
    Texture& operator=(Texture&) = delete;
    /// @brief テクスチャファイルの読み込み
/// @param filePath テクスチャファイルのパス
    static void LoadTexture(const std::string& filePath);


};

