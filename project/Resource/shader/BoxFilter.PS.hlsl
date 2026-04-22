#include "Fullscreen.hlsli"

struct Material
{
    float kernel;
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
    uint32_t width, height;
    gTexture.GetDimensions(width, height);
    float32_t2 uvStepSize = float32_t2(rcp(width), rcp(height));
    
    PixelShaderOutput output;
    
    output.color.rgb = float3(0.0f, 0.0f, 0.0f);
    output.color.a = 1.0f;
     
    //if you input kernel 1 : 2*x+1 = 3 kKernel = 1.0f/9.0f;
    const float32_t kKernel = { 1.0f / pow(2*gMaterial.kernel+1, 2.0f) };

    for (int32_t x = -gMaterial.kernel; x <= gMaterial.kernel; ++x)
    {
        for (int32_t y = -gMaterial.kernel; y <= gMaterial.kernel; ++y)
        {
            float2 index;
            index.x = (float32_t) x;
            index.y = (float32_t) y;
 
            float32_t2 texcoord = input.texcoord + index * uvStepSize;
            float32_t3 fetchColor = gTexture.Sample(gSampler, texcoord).rgb;
            output.color.rgb += fetchColor * kKernel;

        }
    }
    
    return output;
}