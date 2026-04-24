
#include "../Hlsli/Fullscreen.hlsli"


struct Material
{
    float4 color;
    bool useEffect;
    int type;
    bool padding[2];
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
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
  
    if (gMaterial.useEffect)
    {
        float32_t value = dot(output.color.rgb, float32_t3(0.2125f, 0.715f, 0.0721f));
        
        if (gMaterial.type == 0)
        {
        //sepia
            output.color.rgb = value * gMaterial.color.rgb;
        }
        else if (gMaterial.type == 1)
        {
        //grayscale
            output.color.rgb = float32_t3(value, value, value);
        }
    }
   

    return output;
}