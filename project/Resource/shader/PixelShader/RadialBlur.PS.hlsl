#include "../Hlsli/Fullscreen.hlsli"

struct Material
{
    float32_t2 center;
    int32_t numSamples;
    float32_t blurWidth;
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t2);
SamplerState gSampler : register(s0);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
        
    float32_t2 direction = input.texcoord - gMaterial.center;
    float32_t3 outputColor = float32_t3(0.0f, 0.0f, 0.0f);
    
    for (int32_t sampleIndex = 0; sampleIndex < gMaterial.numSamples; ++sampleIndex)
    {
        float32_t2 texcoord = input.texcoord + direction * gMaterial.blurWidth * float32_t(sampleIndex);
        outputColor.rgb += gTexture.Sample(gSampler, texcoord).rgb;
        
    }
    
    outputColor.rgb *= rcp(gMaterial.numSamples);
    
    PixelShaderOutput output;
    output.color.rbg = outputColor;
    output.color.a = 1.0f;
    return output;
}