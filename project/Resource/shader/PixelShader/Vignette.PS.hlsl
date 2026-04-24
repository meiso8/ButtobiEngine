#include "../Hlsli/Fullscreen.hlsli"

struct Material
{
    float correctVal;
    float viignetteVal;
    float padding[2];
};


Texture2D<float4> gTexture : register(t2);
SamplerState gSampler : register(s0);
ConstantBuffer<Material> gMaterial : register(b0);


struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    float32_t2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    float vignette = correct.x * correct.y * gMaterial.correctVal;
    vignette = saturate(pow(vignette, gMaterial.viignetteVal));
    output.color.rgb *= vignette;
    return output;
}