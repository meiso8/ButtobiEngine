#include "Texture.h"
#include"DirectXCommon.h"
#include"StringUtility.h"
#include<cassert>
#include"SRVmanager/SrvManager.h"


std::vector<uint32_t> Texture::handles;
std::unordered_map<uint32_t, std::string> Texture::handleToPath_;
using namespace StringUtility;

std::unordered_map<std::string, Texture::TextureData> Texture::textureDatas;

void Texture::Initialize()
{
    textureDatas.reserve(SrvManager::kMaxSRVCount);

    // handles 配列を初期化（未ロード状態を示すために 0 で埋める）
    handles.resize(TEXTURES, 0);
}


void Texture::LoadAllTexture() {

    handles.resize(TEXTURES);
    handles[WHITE_1X1] = Load("Resource/Textures/white1x1.png");
    handles[UV_CHECKER] = Load("Resource/Textures/uvChecker.png");
    handles[NUMBERS] = Load("Resource/Textures/numbers.png");

    handles[TITLE] = Load("Resource/Textures/Title.png");
    handles[CREDIT] = Load("Resource/Textures/credit.png");
    handles[BUTTON_EXIT] = Load("Resource/Textures/exit.png");
    handles[BUTTON_START] = Load("Resource/Textures/start.png");

    handles[BUTTON_BACK_TO_GAME] = Load("Resource/Textures/back.png");
    handles[BUTTON_BACK_TO_TITL] = Load("Resource/Textures/titleButton.png");

    handles[TEST3] = Load("Resource/Textures/test3.png");

    handles[MEMO1] = Load("Resource/Textures/memo01.png");
    handles[MEMO2] = Load("Resource/Textures/memo02.png");
    handles[MEMO3] = Load("Resource/Textures/memo03.png");
    handles[MEMO4] = Load("Resource/Textures/memo04.png");

    handles[BOOK] = Load("Resource/Textures/book.png");
    handles[BOOK2] = Load("Resource/Textures/book02.png");
    handles[BOOK3] = Load("Resource/Textures/book03.png");
    handles[BOOK4] = Load("Resource/Textures/book04.png");


    handles[NONE] = Load("Resource/Textures/none.png");
    handles[PUZZLE] = Load("Resource/Textures/puzzle.png");
    handles[PUZZLE_NUM] = Load("Resource/Textures/puzzleNum.png");

    handles[HIERO_S] = Load("Resource/Textures/s.png");
    handles[HIERO_P] = Load("Resource/Textures/p.png");
    handles[HIERO_D] = Load("Resource/Textures/d.png");
    handles[HIERO_T] = Load("Resource/Textures/t.png");
    handles[BD_HUNEFER] = Load("Resource/Textures/BD_Hunefer.jpg");

    handles[ENDING1] = Load("Resource/Textures/ending01.png");
    handles[ENDING2] = Load("Resource/Textures/ending02.png");

    handles[SLOT] = Load("Resource/Textures/slot.png");
    handles[EYE] = Load("Resource/Textures/eye.png");
    handles[WORLD] = Load("Resource/Textures/world.png");
}

uint32_t Texture::Load(const std::string& filePath)
{
    LoadTexture(filePath);

    uint32_t index = GetSrvIndexByFilePath(filePath);
    handleToPath_[index] = filePath;
    return index;
}

uint32_t Texture::AddTextureHandle(const std::string& filePath) {

    uint32_t srvIndex = Load(filePath);
    // すでに登録済みならそのSRVインデックスを返す
    for (uint32_t existing : handles) {
        if (existing == srvIndex) {
            return srvIndex;
        }
    }

    handles.push_back(srvIndex);
    return srvIndex;
}

Texture::TEXTURE_HANDLE Texture::GetTextureHandle(const uint32_t& srvIndex)
{
    auto it = std::find(handles.begin(), handles.end(), srvIndex); 
    if (it != handles.end()) { 
        // インデックスを取得して enum にキャスト 
       size_t index = std::distance(handles.begin(), it);
       return static_cast<TEXTURE_HANDLE>(index);
    }
    assert(it != handles.end());

    // 見つからなかった場合のフォールバック（適宜変更）
    return Texture::WHITE_1X1;
}

void Texture::Finalize()
{
    textureDatas.clear();
}

void Texture::LoadTexture(const std::string& filePath)
{
    //読み込み済みテクスチャを検索
    if (textureDatas.contains(filePath)) {
        return;
    }

    //テクスチャ枚数上限チェック
    assert(SrvManager::IsMaxCount());

    //テクスチャファイルを読んでプログラムで扱えるようにする
    DirectX::ScratchImage image{};
    std::wstring filePathW = ConvertString(filePath);
    //sRBG空間で作られた物として読む。
    HRESULT hr = DirectX::LoadFromWICFile(filePathW.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
    assert(SUCCEEDED(hr));

    const DirectX::TexMetadata metadata = image.GetMetadata();

    //ミニマップの作成
    DirectX::ScratchImage mipImages{};

    if (metadata.width > 1 || metadata.height > 1) {
        hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
        assert(SUCCEEDED(hr));
    } else {
        mipImages = std::move(image); // そのまま使う
    }

    //追加したテクスチャデータの参照を取得する
    TextureData& textureData = textureDatas[filePath];

    textureData.filePath = filePath;
    textureData.metadata = mipImages.GetMetadata();
    textureData.resource = DirectXCommon::CreateTextureResource(textureData.metadata);
    textureData.intermediateResource = DirectXCommon::UploadTextureData(textureData.resource, mipImages);

    //テクスチャデータの要素数番号をSRVのインデックスとする
    textureData.srvIndex = SrvManager::Allocate();

    textureData.srvHandleCPU = SrvManager::GetCPUDescriptorHandle(textureData.srvIndex);
    textureData.srvHandleGPU = SrvManager::GetGPUDescriptorHandle(textureData.srvIndex);

    SrvManager::CreateSRVforTexture2D(textureData.srvIndex, textureData.resource.Get(), textureData.metadata.format, UINT(textureData.metadata.mipLevels));

}

uint32_t Texture::GetSrvIndexByFilePath(const std::string& filePath)
{

    //読み込み済みテクスチャを検索
    if (textureDatas.contains(filePath)) {
        return textureDatas.at(filePath).srvIndex;
    }

    assert(0);
    return 0;
}


D3D12_GPU_DESCRIPTOR_HANDLE Texture::GetSrvHandleGPU(const std::string& filePath)
{

    //テクスチャ番号が正常範囲内にある
    assert(SrvManager::IsMaxCount());
    //テクスチャデータの参照を取得
    return textureDatas[filePath].srvHandleGPU;
}

const DirectX::TexMetadata& Texture::GetMetaData(const uint32_t& handle)
{
    //テクスチャ番号が正常範囲内にある
    assert(SrvManager::IsMaxCount());

    return textureDatas[handleToPath_[handle]].metadata;
}
