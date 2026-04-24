#pragma once

#include <dxcapi.h>
#include<string>
#include<wrl.h>
#include<vector>

//CompileShader関数
enum ShaderType {
    kNormal,
    kParticle,
    kSprite,
    kSkinning,
    kFont,
    kSkyBox,
    kOffScreen,
    kGrayScale,
    kVignette,
    kBoxFilter,
    Shaders,
};

class DxcCompiler
{
public:

    void Initialize();
    void ShaderSetting();
    Microsoft::WRL::ComPtr <IDxcBlob>& GetVertexShaderBlob(uint32_t index) { return vertexShaderBlobs_[index]; };
    Microsoft::WRL::ComPtr <IDxcBlob>& GetPixelShaderBlob(uint32_t index) { return pixelShaderBlobs_[index]; };
private:
    IDxcUtils* dxcUtils_ = nullptr;
    IDxcCompiler3* dxcCompiler_ = nullptr;
    IDxcIncludeHandler* includeHandler_ = nullptr;

    std::vector<Microsoft::WRL::ComPtr<IDxcBlob>> vertexShaderBlobs_;
    std::vector<Microsoft::WRL::ComPtr<IDxcBlob>>pixelShaderBlobs_;

private:
    Microsoft::WRL::ComPtr<IDxcBlob>CompileShader(
        //CompilerするShaderファイルへのパス
        const std::wstring& filePath,
        //Compilerに使用するProfile
        const wchar_t* profile);

};