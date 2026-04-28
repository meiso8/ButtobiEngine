
#include "../Hlsli/Fullscreen.hlsli"

struct Material
{
    float maskVal;
};

ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float> gMaskTexture : register(t1);
Texture2D<float4> gTexture : register(t2);
SamplerState gSampler : register(s0);


struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    float mask = gMaskTexture.Sample(gSampler, input.texcoord);
    
    if (mask <= gMaterial.maskVal)
    {
        discard;
    }
    
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    return output;
}