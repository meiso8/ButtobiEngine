#include"FreeTypeManager.h"
#include<cstdint>
#include<iostream>
#include<fstream>
#include<cassert>
#include"DirectXCommon.h"
#include"SRVmanager/SrvManager.h"
#include"Log.h"

using namespace std;


std::unordered_map<uint32_t, FTTextureData>FreeTypeManager::glyphTextures_;

void FreeTypeManager::Initialize()
{
    FT_Error error = FT_Init_FreeType(&library_);
    if (error == 0) { 
        DebugLog("FT_Init_FreeType Success!\n");
    } else {
        std::string msg = "FT_Init_FreeType failed!: " + std::to_string(error) + "\n";
        DebugLog(msg);
    }
}

void FreeTypeManager::SetTTF(const string& fontPath, const uint32_t index, const uint32_t faceIndex)
{

    faces_.resize(faceIndex + 1);

    // ファイルをバイナリで開く
    std::ifstream file(fontPath, std::ios::binary | std::ios::ate);
    if (!file) {
        std::string msg = "Cannot Open File: " + fontPath + "\n";
        DebugLog(msg);
        return;
    }

    // ファイルサイズを取得してバッファ確保
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    fontData_.resize(faceIndex + 1); // 必要なサイズに拡張
    fontData_[faceIndex].resize(static_cast<size_t>(size));

    if (!file.read(reinterpret_cast<char*>(fontData_[faceIndex].data()), size)) {
        DebugLog("CannotLoadFontData\n");
        return;
    }

    FT_Error err = FT_New_Memory_Face(
        library_,
        fontData_[faceIndex].data(),
        static_cast<FT_Long>(fontData_[faceIndex].size()),
        index,
        &faces_[faceIndex]);

    if (err == FT_Err_Unknown_File_Format)
    {
        DebugLog("FT_Err_Unknown_File_Format\n");
        return;
    } else if (err)
    {
        std::string msg = "FT_New_Memory_Face failed!: " + std::to_string(err) + "\n";
        DebugLog(msg);
        return;
    }



}

void FreeTypeManager::Finalize()
{

    for (auto& glyphTexture : glyphTextures_) {
      
        if (glyphTexture.second.ftData.resource != nullptr) {
            glyphTexture.second.ftData.resource.Reset();
            glyphTexture.second.ftData.resource = nullptr;
        }

        if (glyphTexture.second.ftData.intermediateResource != nullptr) {
            glyphTexture.second.ftData.intermediateResource.Reset();
            glyphTexture.second.ftData.intermediateResource = nullptr;
        }

    }

    glyphTextures_.clear();

    //faceの破棄
    for (auto& face : faces_) {
        FT_Done_Face(face);
    }
    faces_.clear();
    //libraryの破棄
    FT_Done_FreeType(library_);
}

FreeTypeManager::~FreeTypeManager()
{
    Finalize();
}


void FreeTypeManager::GetBitMapGlyph(uint32_t faceIndex, FT_UInt glyphIndex)
{

    FT_Error err = FT_Load_Glyph(faces_[faceIndex],  glyphIndex, FT_LOAD_DEFAULT| FT_LOAD_COLOR);
    if (err) {
        DebugLog("FT_Load_Glyph faild\n");
        return;
    }

    //(オプション)face->glyphをラスタライズする
    if (FT_Render_Glyph(faces_[faceIndex]->glyph, FT_RENDER_MODE_NORMAL)) {
        DebugLog("FT_Render_Glyph failed!\n");
        return;
    }

    //このときface->glyph->format == FT_GLYPH_FORMAT_BITMAP && 
    //    face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_BGRAならカラー絵文字（PNG）
    //    face->glyph->bitmapにBGRAビットマップが入ってる
}

void FreeTypeManager::GetOutLineGlyph(uint32_t faceIndex, FT_UInt glyphIndex, uint32_t width, uint32_t height)
{

    //生のアウトラインデータを取得する場合
    FT_Load_Glyph(faces_[faceIndex], glyphIndex, FT_LOAD_NO_SCALE | FT_LOAD_NO_AUTOHINT | FT_LOAD_NO_BITMAP);

    if (faces_[faceIndex]->glyph->format != FT_GLYPH_FORMAT_OUTLINE) {
        DebugLog("This Glyph is not OutLineGlyph type\n");
        return;
    }

    //サイズにスケーリング・ヒンティングしたものを取得する場合
    if (FT_Set_Pixel_Sizes(faces_[faceIndex], width, height)) { DebugLog("Failed to Set_Pixel_Sizes\n"); return; }

    //グリフの読み込み
    if (FT_Load_Glyph(faces_[faceIndex], glyphIndex, FT_LOAD_NO_BITMAP)) { DebugLog("Failed to Load_Glyph\n"); return; }

    //face->glyph->outlineにアウトラインが入ってる
    if (!faces_[faceIndex]) {
        std::string msg = "face[" + std::to_string(faceIndex) + "] is invalid\n";
        DebugLog(msg);

        return;
    }

    //(オプション)face->glyph->outlineをラスタライズする
    if (FT_Render_Glyph(faces_[faceIndex]->glyph, FT_RENDER_MODE_LCD)) { DebugLog("Glyph rasterization failed!\n"); return; }

    //FT_RENDER_MODE_NORMAL = 0,
    //    FT_RENDER_MODE_LIGHT,
    //    FT_RENDER_MODE_MONO,
    //    FT_RENDER_MODE_LCD,
    //    FT_RENDER_MODE_LCD_V,
    //    FT_RENDER_MODE_SDF,
}

void FreeTypeManager::Draw(uint32_t faceIndex)
{

    FT_Bitmap& bmp = faces_[faceIndex]->glyph->bitmap;
    int width = bmp.width;
    int height = bmp.rows;
    unsigned char* buffer = bmp.buffer;

    // 例：ピクセルをコンソールに描画（デバッグ用）
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char value = buffer[y * bmp.pitch + x];
            std::cout << (value > 128 ? '#' : ' ');
        }
        std::cout << '\n';
    }

    if (faceIndex >= sprites_.size() || !sprites_[faceIndex]) {
        DebugLog("Sprite does not exist!faceIndex" + std::to_string(faceIndex) + "\n");
        return;
    }


    Sprite::PreDraw();
    sprites_[faceIndex]->Draw();
}

void FreeTypeManager::ShowFontSize(uint32_t faceIndex)
{
    FT_Face face = faces_[faceIndex];
    for (int i = 0; i < face->num_fixed_sizes; ++i) {
        std::cout << "Size[" << i << "]: " << face->available_sizes[i].width << "x" << face->available_sizes[i].height << "\n";
    }
}


FTTextureData FreeTypeManager::CreateTextureFromFTBitmap(const FT_Bitmap& bitmap) {
   
    FTTextureData result;
    result.ftData = CreateTextureResourceByBitMap(bitmap);

    // 3. アップロードバッファにビットマップを書き込む
    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = bitmap.buffer;
    subResourceData.RowPitch = std::abs(bitmap.pitch);
    subResourceData.SlicePitch = std::abs(bitmap.pitch) * bitmap.rows;

    if (bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) {
        DebugLog("bitmap.pixel_mode is not GRAY!\n");
    }

    auto cmdList = CommandList::GetCommandList();
    UpdateSubresources(cmdList.Get(), result.ftData.resource.Get(), result.ftData.intermediateResource.Get(), 0, 0, 1, &subResourceData);

    // 4. コピー先を PIXEL_SHADER_RESOURCE に遷移 テクスチャをシェーダーで使える状態に切り替える
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        result.ftData.resource.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
    );
    cmdList->ResourceBarrier(1, &barrier);


    return result;
}

FTData FreeTypeManager::CreateTextureResourceByBitMap(const FT_Bitmap& bitmap)
{
    using namespace Microsoft::WRL;

    FTData result;

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

    return result;
}

void FreeTypeManager::CreateSprite(uint32_t faceIndex)
{
    auto& bitmap = faces_[faceIndex]->glyph->bitmap;

    if (!bitmap.buffer) {
        DebugLog("bitmap.buffer is null\n");
        assert(bitmap.buffer);
    }

    glyphTextures_[faceIndex] = CreateTextureFromFTBitmap(bitmap);

    if (glyphTextures_[faceIndex].ftData.resource) {

        uint32_t srvIndex = SrvManager::Allocate();
        glyphTextures_[faceIndex].srvIndex = srvIndex;
        Texture::AddTextureHandleByIndex(srvIndex);

        glyphTextures_[faceIndex].srvHandleCPU = SrvManager::GetCPUDescriptorHandle(srvIndex);
        glyphTextures_[faceIndex].srvHandleGPU = SrvManager::GetGPUDescriptorHandle(srvIndex);

        SrvManager::CreateSRVforTexture2D(srvIndex, glyphTextures_[faceIndex].ftData.resource.Get(), DXGI_FORMAT_R8_UNORM, 1);
         
        sprites_.resize(faceIndex + 1);
        sprites_[faceIndex] = std::make_unique<Sprite>();
        sprites_[faceIndex]->Create(Texture::GetTextureHandle(srvIndex), { 100.0f, 100.0f });
        sprites_[faceIndex]->SetSize({ (float)bitmap.width, (float)bitmap.rows });

    }

}

void FreeTypeManager::SetPixelSizes(uint32_t faceIndex, uint32_t width, uint32_t height)
{

    if (!faces_[faceIndex]) { DebugLog("face is null!\n"); return; }

    if (FT_Set_Pixel_Sizes(faces_[faceIndex], width, height)) { DebugLog("Failed to Set_Pixel_Sizes\n"); return; }
}

FT_UInt FreeTypeManager::GetGlyphID(uint32_t faceIndex, uint32_t unicode, uint32_t uvs)
{
    //Unicodeのコードポイントから
    FT_UInt result = FT_Face_GetCharVariantIndex(faces_[faceIndex], unicode, uvs);

    if (result == 0) {
        //0が帰ってきたらフォールバックする

        FT_UInt glyphIndex = FT_Get_Char_Index(faces_[faceIndex], unicode);

        if (glyphIndex == 0) {
            DebugLog("Glyph not found\n");
            assert(glyphIndex != 0);
        } 

        return glyphIndex;

    } else {
        return result;
    }

}

void FreeTypeManager::GetBitMap(uint32_t faceIndex, FT_UInt glyphIndex, FT_Int strikeIndex)
{
    //ビットマップグリフを取得する
    FT_Select_Size(faces_[faceIndex], strikeIndex);

    FT_Load_Glyph(faces_[faceIndex], glyphIndex, FT_LOAD_DEFAULT);
    /*   このときface->glyph->format == FT_GLYPH_FORMAT_BITMAPなら埋め込みビットマップ
           face->glyph->bitmapに（多くの場合モノクロの）ビットマップが入ってる*/


}
