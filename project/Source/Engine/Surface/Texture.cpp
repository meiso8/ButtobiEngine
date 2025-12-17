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
    handles[WHITECIRCLE] = Load("Resource/Textures/WhiteCircle.png");
    handles[UV_CHECKER] = Load("Resource/Textures/uvChecker.png");
    handles[NUMBERS] = Load("Resource/Textures/Numbers_Pop.png");

    handles[PLAYER_HP_ICON] = Load("Resource/Textures/PlayerHPIcon.png");
    handles[GAUGE01] = Load("Resource/Textures/BossHPGauge_Pop.png");
    handles[GAUGE02] = Load("Resource/Textures/BossHPGauge02_Pop.png");

    handles[PLAYER_WALK_PARTICLE] = Load("Resource/Textures/PlayerWalkParticle_Pop.png");
    handles[STAR_PARTICLE] = Load("Resource/Textures/PlayerDamageParticle.png");
    handles[ENEMY_ACTION_PARTICLE] = Load("Resource/Textures/EnemyActionParticle.png");

    handles[WIND_ATTACK_PARTICLE01] = Load("Resource/Textures/WindAttackParticle01.png");
    handles[WIND_ATTACK_PARTICLE02] = Load("Resource/Textures/WindAttackParticle02.png");
    handles[MELT_FLOOR_PARTICLE] = Load("Resource/Textures/MeltFloorParticle.png");
    handles[LEAF_PARTICLE] = Load("Resource/Textures/LeafParticle.png");
    handles[FLOOR_PARTICLE] = Load("Resource/Textures/FloorParticle.png");
    handles[BUBBLE_PARTICLE] = Load("Resource/Textures/BubbleParticle03.png");
    handles[MAD_PARTICLE] = Load("Resource/Textures/MadParticle.png");

    handles[HEAL_PARTICLE] = Load("Resource/Textures/HealParticle.png");
    handles[COMMON_PARTICLE] = Load("Resource/Textures/CommonParticle01.png");

    handles[BLOOM] = Load("Resource/Textures/Bloom.png");

    handles[ENEMY_TEXTURE2] = Load("Resource/Models/Enemy/Enemy02.png");
    handles[ENEMY_TEXTURE3] = Load("Resource/Models/Enemy/Enemy03.png");
    handles[KARAMATSU] = Load("Resource/Textures/karamatsu.png");
    
	handles[NOSE_LANTERN_PARTICLE] = Load("Resource/Textures/BubbleParticle.png");

    handles[ACTION_UI_MOVE] = Load("Resource/Textures/ActionUI_Move_Pop.png");
    handles[ACTION_UI_STRIP] = Load("Resource/Textures/ActionUI_Strip_Pop.png");
    handles[ACTION_UI_SHOT] = Load("Resource/Textures/ActionUI_Shot_Pop.png");
    handles[ACTION_UI_MENU] = Load("Resource/Textures/PauseUI_Pop.png");

    handles[PAUSING_BG] = Load("Resource/Textures/PausingBG_03.png");
    handles[PAUSING] = Load("Resource/Textures/PauseUI_Word.png");

    handles[BUTTON_BACK_TO_TITL] = Load("Resource/Textures/BackToTitleUI_Pop.png");
    handles[BUTTON_BACK_TO_GAME] = Load("Resource/Textures/BackToGameUI_Pop.png");
    handles[BUTTON_RETRY] = Load("Resource/Textures/RetryUI_Pop.png");
    handles[BUTTON_A] = Load("Resource/Textures/Button_A_Pop.png");
    handles[CONFIRM_UI] = Load("Resource/Textures/ConfirmUI_Pop.png");

    handles[RESULT_CLEAR] = Load("Resource/Textures/GameClear.png");
    handles[RESULT_GAMEOVER] = Load("Resource/Textures/GameOver.png");

    handles[GAMEOVER_STRING] = Load("Resource/Textures/GameOverString.png");
    handles[GAMEOVER_TIPS_1] = Load("Resource/Textures/Tips01.png");
	  handles[GAMEOVER_TIPS_2] = Load("Resource/Textures/Tips02.png");
	  handles[GAMEOVER_TIPS_3] = Load("Resource/Textures/Tips03.png");
	  handles[GAMEOVER_TIPS_4] = Load("Resource/Textures/Tips04.png");
	  handles[GAMEOVER_TIPS_5] = Load("Resource/Textures/Tips05.png");
	  handles[GAMEOVER_TIPS_6] = Load("Resource/Textures/Tips06.png");
	  handles[GAMEOVER_TIPS_7] = Load("Resource/Textures/Tips07.png");
	  handles[GAMEOVER_TIPS_8] = Load("Resource/Textures/Tips08.png");
	  handles[GAMEOVER_TIPS_9] = Load("Resource/Textures/Tips09.png");
	  handles[GAMEOVER_TIPS_10] = Load("Resource/Textures/Tips10.png");

      handles[SKY_DOME] = Load("Resource/Textures/skydome.png");
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
