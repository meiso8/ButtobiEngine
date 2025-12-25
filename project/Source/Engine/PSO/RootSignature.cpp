#include "RootSignature.h"

#include<cassert>

#include"Log.h"

#include<iostream>
#include"DirectXCommon.h"

void RootSignature::Create() {

#pragma region//rootSignature
    //rootSignature作成
    D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature[TYPES]{};
    for (uint32_t i = 0; i < TYPES; ++i) {
        descriptionRootSignature[i].Flags =
            D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    }

#pragma endregion

#pragma region//DescriptorRange
    //DescriptorRange
    D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
    descriptorRange[0].BaseShaderRegister = 2;//2から始める Texture2D<float32_t4> gTexture : register(t2); 
    descriptorRange[0].NumDescriptors = 1;//1つ
    descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRV
    descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//オフセット自動計算

    //Instancing用
    D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
    descriptorRangeForInstancing[0].BaseShaderRegister = 3; // gTransformationMatrices : register(t3)
    descriptorRangeForInstancing[0].NumDescriptors = 1;
    descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


    //PointLight用
    D3D12_DESCRIPTOR_RANGE descriptorRangeForPointLight[1] = {};
    descriptorRangeForPointLight[0].BaseShaderRegister = 4; // : register(t4)
    descriptorRangeForPointLight[0].NumDescriptors = 1;
    descriptorRangeForPointLight[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRangeForPointLight[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    //MatrixPalette用
    D3D12_DESCRIPTOR_RANGE descriptorRangeForMatrixPalette[1] = {};
    descriptorRangeForMatrixPalette[0].BaseShaderRegister = 5;// StructuredBuffer<Well> gMatrixPalette : register(t5);
    descriptorRangeForMatrixPalette[0].NumDescriptors = 1;
    descriptorRangeForMatrixPalette[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    descriptorRangeForMatrixPalette[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;



    //D3D12_DESCRIPTOR_RANGE waveDescriptorRange[1] = {};
    //waveDescriptorRange[0].BaseShaderRegister = 1; // StructuredBuffer<Wave> gWave : register(t1);
    //waveDescriptorRange[0].NumDescriptors = 1;
    //waveDescriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
    //waveDescriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

#pragma endregion

#pragma region//Sampler
    //Smaplerの設定
    D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
    staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;//バイナリフィルタ
    staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;//0-1の範囲外をリピート
    staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;//比較せぬ
    staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;//ありったけのMipmapを使う
    staticSamplers[0].ShaderRegister = 0;
    staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う

    for (uint32_t i = 0; i < TYPES; ++i) {
        //同じサンプラーをセットする
        descriptionRootSignature[i].pStaticSamplers = staticSamplers;
        descriptionRootSignature[i].NumStaticSamplers = _countof(staticSamplers);
    }

#pragma endregion

#pragma region//NormalRootParameters
    //CBufferを利用することになったので、RootParameterに設定を追加する
   /* RootParameter作成。PixelShaderのMaterialとVertexShaderのTransform*/
    D3D12_ROOT_PARAMETER rootParameters[9] = {};
    //Material b0
    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号0を使う
    //Transform用 b0
    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
    rootParameters[1].Descriptor.ShaderRegister = 0;//レジスタ番号0を使う
    //Texture t2
    rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Table
    rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
    rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数
    //DirectionalLight b1
    rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
    rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParameters[3].Descriptor.ShaderRegister = 1;//レジスタ番号1を使う
    //Wave t1
    rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;//SRVを使う
    rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
    rootParameters[4].Descriptor.ShaderRegister = 1;//レジスタ番号1を使う

    //rootParameters[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    //rootParameters[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
    //rootParameters[4].DescriptorTable.pDescriptorRanges = waveDescriptorRange;
    //rootParameters[4].DescriptorTable.NumDescriptorRanges = _countof(waveDescriptorRange);

    //Ballon b1
    rootParameters[5].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
    rootParameters[5].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
    rootParameters[5].Descriptor.ShaderRegister = 1;//レジスタ番号1を使う

    //Camera
    rootParameters[6].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
    rootParameters[6].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParameters[6].Descriptor.ShaderRegister = 2;//レジスタ番号2を使う
    //PointLight t4
    rootParameters[7].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Table
    rootParameters[7].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParameters[7].DescriptorTable.pDescriptorRanges = descriptorRangeForPointLight;//Tableの中身の配列を指定
    rootParameters[7].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForPointLight);//Tableで利用する数

    //DirectionalLight b3
    rootParameters[8].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
    rootParameters[8].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParameters[8].Descriptor.ShaderRegister =3;//レジスタ番号1を使う


#pragma endregion

#pragma region//rootParametersForSkinning

    D3D12_ROOT_PARAMETER rootParametersForSkinning[10] = {};

    for (int i = 0; i < 9; ++i) {
        rootParametersForSkinning[i] = rootParameters[i];
    }

    //MatrixPalette t5
    rootParametersForSkinning[9].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Table
    rootParametersForSkinning[9].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
    rootParametersForSkinning[9].DescriptorTable.pDescriptorRanges = descriptorRangeForMatrixPalette;//Tableの中身の配列を指定
    rootParametersForSkinning[9].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForMatrixPalette);//Tableで利用する数
#pragma endregion



#pragma region//ParticleRootParameters
    //CBufferを利用することになったので、RootParameterに設定を追加する
   /* RootParameter作成。PixelShaderのMaterialとVertexShaderのTransform*/
    D3D12_ROOT_PARAMETER rootParametersForInstancing[3] = {};
    //Material b0
    rootParametersForInstancing[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
    rootParametersForInstancing[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParametersForInstancing[0].Descriptor.ShaderRegister = 0;//レジスタ番号0を使う

    //Transform用 t3
    rootParametersForInstancing[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Table
    rootParametersForInstancing[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
    rootParametersForInstancing[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;//レジスタ番号0を使う
    rootParametersForInstancing[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);//Tableで利用する数
    //Texture? t2
    rootParametersForInstancing[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Table
    rootParametersForInstancing[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParametersForInstancing[2].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
    rootParametersForInstancing[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数
#pragma endregion


#pragma region//SpriteParameters
    //CBufferを利用することになったので、RootParameterに設定を追加する
   /* RootParameter作成。PixelShaderのMaterialとVertexShaderのTransform*/
    D3D12_ROOT_PARAMETER rootParametersForSprite[3] = {};
    //Material
    rootParametersForSprite[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
    rootParametersForSprite[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParametersForSprite[0].Descriptor.ShaderRegister = 0;//レジスタ番号0を使う
    //Transform用
    rootParametersForSprite[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
    rootParametersForSprite[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
    rootParametersForSprite[1].Descriptor.ShaderRegister = 0;//レジスタ番号0を使う
    //Texture?
    rootParametersForSprite[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//Table
    rootParametersForSprite[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
    rootParametersForSprite[2].DescriptorTable.pDescriptorRanges = descriptorRange;//Tableの中身の配列を指定
    rootParametersForSprite[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);//Tableで利用する数

#pragma endregion

    descriptionRootSignature[kNormal].pParameters = rootParameters;//ルートパラメータ配列へのポインタ
    descriptionRootSignature[kNormal].NumParameters = _countof(rootParameters);//配列の長さ

    descriptionRootSignature[kParticle].pParameters = rootParametersForInstancing;//ルートパラメータ配列へのポインタ
    descriptionRootSignature[kParticle].NumParameters = _countof(rootParametersForInstancing);//配列の長さ

    descriptionRootSignature[kSprite].pParameters = rootParametersForSprite;//ルートパラメータ配列へのポインタ
    descriptionRootSignature[kSprite].NumParameters = _countof(rootParametersForSprite);//配列の長さ

    descriptionRootSignature[kSkinning].pParameters = rootParametersForSkinning;//ルートパラメータ配列へのポインタ
    descriptionRootSignature[kSkinning].NumParameters = _countof(rootParametersForSkinning);//配列の長さ

    //シリアライズしてバイナリにする
    Microsoft::WRL::ComPtr <ID3DBlob> signatureBlob = nullptr;
    Microsoft::WRL::ComPtr <ID3DBlob> errorBlob = nullptr;

#pragma region//NormalRootParameterシリアライズしてバイナリにする

    for (uint32_t i = 0; i < TYPES; ++i) {

        HRESULT result = D3D12SerializeRootSignature(&descriptionRootSignature[i],
            D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

        if (FAILED(result)) {
            LogFile::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
            assert(false);
        }

        //バイナリ元に生成
        result = DirectXCommon::GetDevice()->CreateRootSignature(0,
            signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
            IID_PPV_ARGS(&rootSignatures_[i]));

        assert(SUCCEEDED(result));
    }

#pragma endregion

}

