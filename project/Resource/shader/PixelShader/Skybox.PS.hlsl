#include "../Hlsli/Skybox.hlsli"

struct Material
{
    float4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);
TextureCube<float4> gTexture : register(t2);
SamplerState gSampler : register(s0);


struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    output.color = textureColor * gMaterial.color;
    return output;
}