#pragma once
//freeTypeを使用するためのインクルードヘッダ
#include <ft2build.h>
#include FT_FREETYPE_H

//DirectX
#include<wrl.h>
#include<d3d12.h>

#include<vector>
#include<string>
#include<memory>
#include<unordered_map>
//自作Sprite
#include<Sprite.h>

//FreeTypeのデータ
struct FTData {
    //face
    FT_Face face;
    //フォントデータ
    std::vector<uint8_t> fontData;
};

struct FTResource {
    //リソース
    Microsoft::WRL::ComPtr<ID3D12Resource> resource;
    Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
};

//FreeType用テクスチャデータ
struct FTTextureData {
    FTData ftData;
    FTResource ftResource;
    uint32_t srvIndex;
    D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
    D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
};

class FreeTypeManager {
public:
    /// @brief 初期化
    static void Initialize();
    /// @brief 終了処理
    static void Finalize();
    ~FreeTypeManager();

    /// @brief Faceの生成
    /// @param fontPath .ttfや.tccを読み込む
    /// @param faceIndex .ttf の場合0でOK　.tccの場合は複数存在する
    static uint32_t CreateFace(const std::string& fontPath, const uint32_t index);
    static FT_UInt GetGlyphID(uint32_t handle, uint32_t unicode, uint32_t uvs);
    static void GetBitMap(uint32_t handle, FT_UInt glyphIndex, FT_Int strikeIndex);
    static void GetBitMapGlyph(uint32_t handle, FT_UInt glyphIndex);
    static void GetOutLineGlyph(uint32_t handle, FT_UInt glyphIndex, uint32_t width, uint32_t height);
    static void Draw(uint32_t handle);
    static void CreateSprite(uint32_t handle);
    static void SetPixelSizes(uint32_t handle, uint32_t width, uint32_t height);
    static void ShowFontSize(uint32_t handle);
private:
    /// @brief FreeTypeのテクスチャデータの生成
    /// @param bitmap 
    /// @return 
    static FTResource CreateTextureFromFTBitmap(const FT_Bitmap& bitmap);
private:
    //一つのライブラリで複数のFaceを保持できる
    static FT_Library library_;
    //FreeTypeTextureDataをマッピングして格納する
    static std::unordered_map<uint32_t, FTTextureData> glyphTextures_;
    //ハンドルごとにスプライトを作成する
    static std::unordered_map<uint32_t, std::unique_ptr<Sprite>> sprites_;
};
