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

namespace std {
    template <>
    struct hash<GlyphKey> {
        size_t operator()(const GlyphKey& k) const {
            return hash<uint64_t>()((uint64_t(k.handle) << 32) | k.glyphIndex);
        }
    };
}

struct GlyphRun {
    FT_UInt glyphIndex;
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
    Vector2 glyphSize; 
    float bearingY = 0.0f;
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
    static void Draw(uint32_t handle, FT_UInt glyphIndex);

    static Sprite* CreateSprite(uint32_t handle, FT_UInt glyphIndex);
    static void SetPixelSizes(uint32_t faceHandle, uint32_t width, uint32_t height);
    static void ShowFontSize(uint32_t handle);

    static std::vector<GlyphRun> LayoutString(uint32_t handle, const std::u32string& text, const Vector2& startPos);
    static float GetMaxDescender(uint32_t handle, std::vector<GlyphRun>& runs);
   static Sprite* GetOrCreateSprite(const GlyphKey& key) {
        auto& pool = spritePool_[key];

        // 未使用のスプライトを探す
        for (auto& sprite : pool) {
            if (!sprite->IsInUse()) {
                sprite->SetInUse(true);
                return sprite.get();
            }
        }

        return CreateSprite(key.handle,key.glyphIndex);
    }

    static void ResetSpriteUsage() {
        for (auto& [key, pool] : spritePool_) {
            for (auto& sprite : pool) {
                sprite->SetInUse(false);
            }
        }
    }
    static const FTTextureData& GetGlyphTextures(const GlyphKey& key);
private:
    /// @brief FreeTypeのテクスチャデータの生成
    /// @param bitmap 
    /// @return 
    static FTResource CreateTextureFromFTBitmap(const FT_Bitmap& bitmap);
    static void CreateGlyphTexture(uint32_t handle, FT_UInt glyphIndex);
private:
    //一つのライブラリで複数のFaceを保持できる
    static FT_Library library_;
    // フォントごとのFTData（faceとfontData）
    static std::unordered_map<uint32_t, FTData> fontFaces_;
    // グリフごとのテクスチャとスプライト
    static std::unordered_map<GlyphKey, FTTextureData> glyphTextures_;
   static std::unordered_map<GlyphKey, std::vector<std::unique_ptr<Sprite>>> spritePool_;

};
