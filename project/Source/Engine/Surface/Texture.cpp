#include "Texture.h"
#include"DirectXCommon.h"
#include"StringUtility.h"
#include<cassert>
#include"SRVmanager/SrvManager.h"


std::vector<uint32_t> Texture::handles;
std::unordered_map<uint32_t, std::string> Texture::handleToPath_;
using namespace StringUtility;

std::unordered_map<std::string, Texture::TextureData> Texture::textureDatas;

void Texture::LoadAllTexture() {

    handles.resize(TEXTURES);
    handles[WHITE_1X1] = Load("Resource/Textures/white1x1.png");
    handles[UV_CHECKER] = Load("Resource/Textures/uvChecker.png");
    handles[NUMBERS] = Load("Resource/Textures/Numbers_Pop.png");

    handles[HART] = Load("Resource/Textures/PlayerHPIcon.png");
    handles[PLAYER_WALK_PARTICLE] = Load("Resource/Textures/PlayerWalkParticle_Pop.png");

	handles[ACTION_UI_MOVE] = Load("Resource/Textures/ActionUI_Move_Pop.png");
	handles[ACTION_UI_STRIP] = Load("Resource/Textures/ActionUI_Strip_Pop.png");
	handles[ACTION_UI_SHOT] = Load("Resource/Textures/ActionUI_Shot_Pop.png");

    handles[PAUSING_BG] = Load("Resource/Textures/PausingBG_02.png");
    handles[PAUSING] = Load("Resource/Textures/PauseUI_Pop.png");

    handles[BUTTON_BACK_TO_TITL] = Load("Resource/Textures/BackToTitleUI_Pop.png");
    handles[BUTTON_BACK_TO_GAME] = Load("Resource/Textures/BackToGameUI_Pop.png");
    handles[BUTTON_RETRY] = Load("Resource/Textures/RetryUI_Pop.png");
    handles[BUTTON_A] = Load("Resource/Textures/Button_A_Pop.png");
    handles[CONFIRM_UI] = Load("Resource/Textures/ConfirmUI_Pop.png");

	handles[RESULT_CLEAR] = Load("Resource/Textures/GameOver.png");
	handles[RESULT_GAMEOVER] = Load("Resource/Textures/GameClear.png");
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

void Texture::Finalize()
{
    textureDatas.clear();
}

void Texture::Initialize()
{
    //SRVの和と同数
    textureDatas.reserve(SrvManager::kMaxSRVCount);

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
