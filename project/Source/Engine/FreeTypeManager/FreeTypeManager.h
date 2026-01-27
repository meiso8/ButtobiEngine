#pragma once
//freeTypeを使用するためのインクルードヘッダ
#include <ft2build.h>
//FT_FREETYPE_H はマクロです。
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


struct GlyphKey {
    //フォントの種類のハンドル
    uint32_t handle;
    //文字のインデックス
    FT_UInt glyphIndex;

    bool operator==(const GlyphKey& other) const {
        return handle == other.handle && glyphIndex == other.glyphIndex;
    }
};

//Hash値の設定？
namespace std {
    template <>
    struct hash<GlyphKey> {
        size_t operator()(const GlyphKey& k) const {
            return hash<uint64_t>()((uint64_t(k.handle) << 32) | k.glyphIndex);
        }
    };
}

struct GlyphRun {
    //文字の添え字
    FT_UInt glyphIndex;
    //位置
    Vector2 position;
};

//FreeTypeのデータ
struct FTData {
    //face フォントの種類を格納する
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
    FTResource ftResource;
    uint32_t srvIndex;
    D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
    D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
    //文字のサイズ
    Vector2 glyphSize;
    float bearingY = 0.0f;
};

class FreeTypeManager {
public:
    /// @brief ライブラリの初期化
    static void Initialize();
    /// @brief 終了処理
    static void Finalize();
    ~FreeTypeManager();

    /// @brief Faceの生成
    /// @param fontPath .ttfや.tccを読み込む
    /// @param faceIndex .ttf の場合0でOK　.tccの場合は複数存在する
    static uint32_t CreateFace(const std::string& fontPath, const uint32_t faceIndex);
    /// @brief 文字のIDの取得
    /// @param faceHandle フォントの種類
    /// @param unicode Unicodeで入力する
    /// @param uvs 基本的に0でいい
    /// @return ID
    static FT_UInt GetGlyphID(uint32_t faceHandle, uint32_t unicode, uint32_t uvs);
    /// @brief ビットマップの取得
    /// @param faceHandle 
    /// @param glyphIndex 
    /// @param strikeIndex 
    static void GetBitMap(uint32_t faceHandle, FT_UInt glyphIndex, FT_Int strikeIndex);
    /// @brief 
    /// @param faceHandle 
    /// @param glyphIndex 
    static void GetBitMapGlyph(uint32_t faceHandle, FT_UInt glyphIndex);
    static void GetOutLineGlyph(uint32_t faceHandle, FT_UInt glyphIndex, uint32_t width, uint32_t height);
    static void Draw(uint32_t faceHandle, FT_UInt glyphIndex);
    static bool LoadAndRenderGlyph(FT_Face& face, FT_UInt glyphIndex, FT_Render_Mode mode);

    static Sprite* CreateSprite(uint32_t faceHandle, FT_UInt glyphIndex);
    static void SetPixelSizes(uint32_t faceHandle, uint32_t width, uint32_t height);
    static void ShowFontSize(uint32_t faceHandle);

    static std::vector<GlyphRun> LayoutString(uint32_t faceHandle, const std::u32string& text, const Vector2& startPos);
    static float GetMaxDescender(uint32_t faceHandle, std::vector<GlyphRun>& runs);
    static Sprite* GetOrCreateSprite(const GlyphKey& key);

    /// @brief エンジンでCommandQueueを送った後にリセットする
    static void ResetSpriteUsage();
    static const FTTextureData& GetGlyphTextures(const GlyphKey& key);

private:
    /// @brief FreeTypeのResource生成
    /// @param bitmap bitmapを入れる
    /// @return FTResource resource intermediateResource
    static FTResource CreateResourceFromFTBitmap(const FT_Bitmap& bitmap);
    /// @brief 文字のテクスチャ生成
    /// @param handle 
    /// @param glyphIndex 
    static void CreateGlyphTexture(uint32_t handle, FT_UInt glyphIndex);
    static void ReleaseResource(FTResource& resource);
private:
    //一つのライブラリで複数のFaceを保持できる
    static FT_Library library_;
    // フォント（←注意）ごとのFTData（faceとfontData）
    static std::unordered_map<uint32_t, FTData> fontFaces_;
    // 文字ごとのテクスチャを格納する
    static std::unordered_map<GlyphKey, FTTextureData> glyphTextures_;
    //文字ごとのSpriteを格納する
    static std::unordered_map<GlyphKey, std::vector<std::unique_ptr<Sprite>>> spritePool_;

};
