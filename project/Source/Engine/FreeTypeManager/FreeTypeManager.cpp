#include"FreeTypeManager.h"
#include<cstdint>
#include<fstream>
#include<cassert>
#include"DirectXCommon.h"
#include"SRVmanager/SrvManager.h"
#include"Log.h"

using namespace std;

FT_Library FreeTypeManager::library_;

unordered_map<uint32_t, FTTextureData> FreeTypeManager::glyphTextures_;
unordered_map<uint32_t, unique_ptr<Sprite>> FreeTypeManager::sprites_;

void FreeTypeManager::Initialize()
{
    //ライブラリの初期化　error　== 0 で成功
    FT_Error error = FT_Init_FreeType(&library_);

    if (error == 0) {
        //初期化に成功
        DebugLog("FT_Init_FreeType Success!\n");
    } else {
        //失敗
        std::string msg = "FT_Init_FreeType failed!: " + std::to_string(error) + "\n";
        DebugLog(msg);
        assert(false);
    }

}


uint32_t FreeTypeManager::CreateFace(const string& fontPath, const uint32_t index)
{

    uint32_t handle = static_cast<uint32_t>(glyphTextures_.size());

    if (glyphTextures_.contains(handle)) {
        return handle;
    }

    FTTextureData data;

    {
        // ファイルをバイナリで開く
        std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
        if (!file) {
            std::string msg = "Cannot Open File: " + fontPath + "\n";
            DebugLog(msg);
            assert(false);
        }

        // ファイルサイズを取得してバッファ確保
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        data.ftData.fontData.resize(static_cast<size_t>(size));

        if (!file.read(reinterpret_cast<char*>(data.ftData.fontData.data()), size)) {
            DebugLog("CannotLoadFontData\n");
            assert(false);
        }

    }


    FT_Error err = FT_New_Memory_Face(
        library_,
        data.ftData.fontData.data(),
        static_cast<FT_Long>(data.ftData.fontData.size()),
        index,
        &data.ftData.face);

    if (err == FT_Err_Unknown_File_Format)
    {
        DebugLog("FT_Err_Unknown_File_Format\n");
    } else if (err)
    {
        std::string msg = "FT_New_Memory_Face failed!: " + std::to_string(err) + "\n";
        DebugLog(msg);
    }

    assert(!err);

    //登録！
    glyphTextures_.emplace(handle, std::move(data));

    return handle;

}

void FreeTypeManager::Finalize()
{

    //一旦明示的にglyphTextures_を解放しておく
    for (auto& glyphTexture : glyphTextures_) {

        auto& data = glyphTexture.second;

        auto& resource = data.ftResource.resource;

        if (resource != nullptr) {
            resource.Reset();
            resource = nullptr;
        }

        auto& intermediateResource = data.ftResource.intermediateResource;

        if (intermediateResource != nullptr) {
            intermediateResource.Reset();
            intermediateResource = nullptr;
        }

        //faceの破棄
        FT_Done_Face(data.ftData.face);


    }

    glyphTextures_.clear();

    //一旦明示的に解放しておく
    for (auto& sprite : sprites_) {
        if (sprite.second != nullptr) {
            sprite.second.reset();
            sprite.second = nullptr;
        }
    }
    sprites_.clear();

    //libraryの破棄
    FT_Done_FreeType(library_);
}

FreeTypeManager::~FreeTypeManager()
{
    Finalize();
}

void FreeTypeManager::GetBitMapGlyph(uint32_t faceIndex, FT_UInt glyphIndex)
{

    auto& data = glyphTextures_.at(faceIndex);
    auto& face = data.ftData.face;

    //グリフの読み込み
    if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT | FT_LOAD_COLOR)) {
        DebugLog("FT_Load_Glyph faild\n");
        return;
    }

    auto& glyph = face->glyph;

    //(オプション)face->glyphをラスタライズする
    if (FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL)) {
        DebugLog("FT_Render_Glyph failed!\n");
        assert(false);
    }

    if (glyph->format == FT_GLYPH_FORMAT_BITMAP &&
        glyph->bitmap.pixel_mode == FT_PIXEL_MODE_BGRA) {
        //カラー絵文字（PNG） face->glyph->bitmapにBGRAビットマップが入ってる
        DebugLog("This is color glyph\n");
    }

}

void FreeTypeManager::GetOutLineGlyph(uint32_t handle, FT_UInt glyphIndex, uint32_t width, uint32_t height)
{

    auto& data = glyphTextures_.at(handle);
    auto& face = data.ftData.face;

    //生のアウトラインデータを取得する場合
    if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_NO_SCALE | FT_LOAD_NO_AUTOHINT | FT_LOAD_NO_BITMAP)) { DebugLog("Failed to Load_Glyph\n"); return; }

    if (!face) {
        std::string msg = "face[" + std::to_string(handle) + "] is invalid\n";
        DebugLog(msg);
        assert(false);
    }

    auto& glyph = face->glyph;

    if (glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
        DebugLog("This Glyph is not OutLineGlyph type\n");
        assert(false);
    }

    //サイズにスケーリング・ヒンティングしたものを取得する場合
    if (FT_Set_Pixel_Sizes(face, width, height)) { DebugLog("Failed to Set_Pixel_Sizes\n");  assert(false); }

    //(オプション)glyph->outlineをラスタライズする
    if (FT_Render_Glyph(glyph, FT_RENDER_MODE_LCD)) { DebugLog("Glyph rasterization failed!\n");  assert(false);
    }
}

void FreeTypeManager::Draw(uint32_t faceIndex)
{
    if (!sprites_.contains(faceIndex)) {
        return;
    }

    auto& sprite = sprites_.at(faceIndex);

    Sprite::PreDraw();
    sprite->Draw();
}

void FreeTypeManager::ShowFontSize(uint32_t handle)
{
    auto& data = glyphTextures_.at(handle);
    auto& face = data.ftData.face;

    for (int i = 0; i < face->num_fixed_sizes; ++i) {
        DebugLog("Size[" + to_string(i) + "]:" + to_string(face->available_sizes[i].width) + "x" + to_string(face->available_sizes[i].height) + "\n");
    }
}


FTResource FreeTypeManager::CreateTextureFromFTBitmap(const FT_Bitmap& bitmap)
{
    using namespace Microsoft::WRL;

    FTResource result;

    int width = bitmap.width;
    int height = bitmap.rows;
    DXGI_FORMAT format = DXGI_FORMAT_R8_UNORM;

    auto device = DirectXCommon::GetDevice();

    // 1. デフォルトヒープにテクスチャ作成
    D3D12_RESOURCE_DESC texDesc = {};
    texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    texDesc.Width = width;
    texDesc.Height = height;
    texDesc.DepthOrArraySize = 1;
    texDesc.MipLevels = 1;
    texDesc.Format = format;
    texDesc.SampleDesc.Count = 1;
    texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    texDesc.Flags = D3D12_RESOURCE_FLAG_NONE;


    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_DEFAULT);
    CD3DX12_RESOURCE_DESC texResourceDesc(texDesc);

    //テクスチャリソースの作成
    HRESULT hr = device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &texResourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&result.resource)
    );

    if (FAILED(hr)) {
        DebugLog("Texture creation failed!\n");
    }

    // 2. アップロードヒープにデータをコピー
    UINT64 uploadBufferSize;
    device->GetCopyableFootprints(&texDesc, 0, 1, 0, nullptr, nullptr, nullptr, &uploadBufferSize);

    CD3DX12_HEAP_PROPERTIES uploadHeapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC uploadDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

    //中間リソース用バッファの作成
    hr = device->CreateCommittedResource(
        &uploadHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &uploadDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&result.intermediateResource)
    );

    if (FAILED(hr)) {
        DebugLog("Create　IntermediateResource failed!\n");
    }


    // 3. 中間リソースにビットマップを書き込む
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = bitmap.buffer;
    subResourceData.RowPitch = std::abs(bitmap.pitch);
    subResourceData.SlicePitch = std::abs(bitmap.pitch) * bitmap.rows;

    if (bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) {
        DebugLog("bitmap.pixel_mode is not GRAY!\n");
    }

    auto cmdList = CommandList::GetCommandList();
    UpdateSubresources(cmdList.Get(), result.resource.Get(), result.intermediateResource.Get(), 0, 0, 1, &subResourceData);

    // 4. コピー先を PIXEL_SHADER_RESOURCE に遷移 テクスチャをシェーダーで使える状態に切り替える
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        result.resource.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
    cmdList->ResourceBarrier(1, &barrier);

    return result;
}

void FreeTypeManager::CreateSprite(uint32_t handle)
{
    if (sprites_.contains(handle)) {
        return;
    }

    auto& data = glyphTextures_.at(handle);
    auto& bitmap = data.ftData.face->glyph->bitmap;

    if (!bitmap.buffer) {
        DebugLog("bitmap.buffer is null\n");
        assert(false);
    }

    data.ftResource = CreateTextureFromFTBitmap(bitmap);
    auto& srvIndex = data.srvIndex;

    if (data.ftResource.resource) {

        srvIndex = SrvManager::Allocate();
        Texture::AddTextureHandleByIndex(srvIndex);

        data.srvHandleCPU = SrvManager::GetCPUDescriptorHandle(srvIndex);
        data.srvHandleGPU = SrvManager::GetGPUDescriptorHandle(srvIndex);

        SrvManager::CreateSRVforTexture2D(srvIndex, data.ftResource.resource.Get(), DXGI_FORMAT_R8_UNORM, 1);

        //Spriteの生成
        auto sprite = std::make_unique<Sprite>();
        sprite->Create(Texture::GetTextureHandle(srvIndex), { 100.0f, 100.0f });
        sprite->SetSize({ (float)bitmap.width, (float)bitmap.rows });

        //ハンドルとSpriteをセットにする
        sprites_.insert(std::make_pair(handle, std::move(sprite)));
    }

}

void FreeTypeManager::SetPixelSizes(uint32_t handle, uint32_t width, uint32_t height)
{
    auto& data = glyphTextures_.at(handle);
    auto& face = data.ftData.face;

    if (!face) { DebugLog("face is null!\n"); return; }

    FT_Error err = FT_Set_Pixel_Sizes(face, width, height);
    if (err) {
        DebugLog("Failed to Set_Pixel_Sizes: error code = " + std::to_string(err) + "\n");
        return;
    }
}

FT_UInt FreeTypeManager::GetGlyphID(uint32_t handle, uint32_t unicode, uint32_t uvs)
{


    auto& data = glyphTextures_.at(handle);

    assert(&data);

    auto& face = data.ftData.face;


    //Unicodeのコードポイントから
    FT_UInt result = FT_Face_GetCharVariantIndex(face, unicode, uvs);

    if (result == 0) {
        //0が帰ってきたらフォールバックする

        FT_UInt glyphIndex = FT_Get_Char_Index(face, unicode);

        if (glyphIndex == 0) {
            DebugLog("Glyph not found: U+" + std::to_string(unicode) + "\n");

            // フォールバック文字（例: '?'）
            glyphIndex = FT_Get_Char_Index(face, '?');

            if (glyphIndex == 0) {
                DebugLog("Fallback glyph '?' not found either!\n");
            }

            assert(glyphIndex);
        }

        return glyphIndex;

    } else {
        return result;
    }

}

void FreeTypeManager::GetBitMap(uint32_t handle, FT_UInt glyphIndex, FT_Int strikeIndex)
{

    auto& data = glyphTextures_.at(handle);
    auto& face = data.ftData.face;

    //ビットマップグリフを取得する 
    if (FT_Select_Size(face, strikeIndex)) { assert(false); }
    //グリフを読み込む
    if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT)) { assert(false); }

    //face->glyph->format == FT_GLYPH_FORMAT_BITMAPなら埋め込みビットマップ 
    // face->glyph->bitmapに（多くの場合モノクロの）ビットマップが入ってる


}
