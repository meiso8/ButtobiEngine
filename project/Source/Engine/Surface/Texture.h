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

        PLAYER_HP_ICON,
        GAUGE01,
        GAUGE02,

        PLAYER_WALK_PARTICLE,
        PLAYER_DAMAGE_PARTICLE,
        ENEMY_ACTION_PARTICLE,
        WIND_ATTACK_PARTICLE01,
        WIND_ATTACK_PARTICLE02,
        MELT_FLOOR_PARTICLE,
        LEAF_PARTICLE,
        FLOOR_PARTICLE,
        BLOOM,

        ENEMY_TEXTURE2,

        ACTION_UI_MOVE,
		ACTION_UI_STRIP,
		ACTION_UI_SHOT,
		ACTION_UI_MENU,

        PAUSING_BG,
        PAUSING,
        BUTTON_BACK_TO_TITL,
        BUTTON_BACK_TO_GAME,
        BUTTON_RETRY,
        BUTTON_A,
        CONFIRM_UI,

		RESULT_CLEAR,
		RESULT_GAMEOVER,

        GAMEOVER_STRING,
        GAMEOVER_TIPS_1,

        TEXTURES,
    };
    static void LoadAllTexture();

    static uint32_t AddTextureHandle(const std::string& filePath);

    static uint32_t GetHandle(const TEXTURE_HANDLE& handle) { return handles[handle]; }
    static std::string& GetFilePath(const TEXTURE_HANDLE& handle) { return handleToPath_[handles[handle]]; }

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
    /// @brief テクスチャファイルの読み込み
/// @param filePath テクスチャファイルのパス
    static void LoadTexture(const std::string& filePath);


};

