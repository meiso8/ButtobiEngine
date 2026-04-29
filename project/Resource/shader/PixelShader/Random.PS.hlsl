#include "../Hlsli/Fullscreen.hlsli"

//Texture2D<float4> gTexture : register(t2);

struct Material
{
    float time;
};

ConstantBuffer<Material> gMaterial : register(b0);
SamplerState gSampler : register(s0);

float rand2dTo1d(float2 value, float2 dotDir = float2(12.9898, 78.233))
{
    float2 samllValue = sin(value);
    float random = dot(samllValue, dotDir);
    random = frac(sin(random) * 143758.5453);
    return random;

}

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float random = rand2dTo1d(input.texcoord *gMaterial.time);
    output.color = float4(random,random,random,1.0f);
    return output;
}