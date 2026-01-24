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

//FreeTypeのリソース
struct FTData {
    Microsoft::WRL::ComPtr<ID3D12Resource> resource;
    Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource;
};

//テクスチャデータ
struct FTTextureData {
    FTData ftData;
    uint32_t srvIndex;
    D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
    D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
};

class FreeTypeManager {
public:
     void Initialize();
    void SetTTF(const std::string& fontPath, const uint32_t index, const uint32_t faceIndex);
     void Finalize();
    ~FreeTypeManager();
    FT_UInt GetGlyphID(uint32_t faceIndex, uint32_t unicode, uint32_t uvs);
    void GetBitMap(uint32_t faceIndex, FT_UInt glyphIndex, FT_Int strikeIndex);
    void GetBitMapGlyph(uint32_t faceIndex, FT_UInt glyphIndex);
    void GetOutLineGlyph(uint32_t faceIndex, FT_UInt glyphIndex, uint32_t width, uint32_t height);
    void Draw(uint32_t faceIndex);
    void ShowFontSize(uint32_t faceIndex);
    void CreateSprite(uint32_t faceIndex);
    void SetPixelSizes(uint32_t faceIndex, uint32_t width, uint32_t height);

private:
    static FTTextureData CreateTextureFromFTBitmap(const FT_Bitmap& bitmap);
    static FTData CreateTextureResourceByBitMap(const FT_Bitmap& bitmap);
private:
    //一つのライブラリで複数のFaceを保持できる
    FT_Library library_;
    //フォントを保持
    std::vector<FT_Face> faces_;
    //スプライト
    std::vector<std::unique_ptr<Sprite>>sprites_;
    // 複数のフォントデータを保持する
    std::vector<std::vector<uint8_t>> fontData_;

    //FreeTypeTextureDataをマッピングして格納する
    static std::unordered_map<uint32_t, FTTextureData> glyphTextures_;
};
