#include "../Hlsli/Fullscreen.hlsli"

struct Material
{
    float32_t lineWidth;
    float32_t3 color;
    float32_t thickness;
    float32_t3 padding;
};

ConstantBuffer<Material> gMaterial : register(b0);

Texture2D<float4> gTexture : register(t2);
//empty thread is t8?
Texture2D<float4> gTemperatureTexture : register(t8);

Texture2D<uint> gObjectTexture : register(t12);

SamplerState gSampler : register(s0);
SamplerState gSamplerPoint : register(s1);

static const float kPrewittHorizontalKernel[3][3] =
{
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
    { -1.0f / 6.0f, 0.0f, 1.0f / 6.0f },
};

static const float kPrewittVerticalKernel[3][3] =
{
    { -1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f },
    { 0.0f, 0.0f, 0.0f },
    { 1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f },
};



struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    
    PixelShaderOutput output;
    
    uint width, height;
    gTexture.GetDimensions(width, height);
    float2 uvStepSize = float2(rcp((float) width), rcp((float) height));
    
    int2 centerCoord = int2(input.texcoord * float2(width, height));
    centerCoord = clamp(centerCoord, int2(0, 0), int2(width - 1, height - 1));
    uint centerID = gObjectTexture.Load(int3(centerCoord, 0));
    
    float2 difference = float2(0.0f, 0.0f);
    float outlineMask = 0.0f;
    

    float4 temperature = gTemperatureTexture.Sample(gSamplerPoint, input.texcoord);
   
    if (temperature.b >= 1.0f)
    {
        //1.0fの時　選択状態ではない　ため　マスクをかける
        outlineMask = 1.0f;
    }
    else
    {
        //それ以外の時　例えば
        if (temperature.b <= 0.0f)
        {
            //この時は選択状態のとき
            outlineMask = max(outlineMask, temperature.g);
        }
    }

    if (outlineMask > 0.75f)
    {
        output.color.rgb = gTexture.Sample(gSampler, input.texcoord).rgb;
        output.color.a = 1.0f;
        return output;
    }
    
    for (int x = 0; x < 3; ++x)
    {
        for (int y = 0; y < 3; ++y)
        {
            int2 offset = int2(x - 1, y - 1) * int(round(gMaterial.thickness));
            int2 sampleCoord = clamp(centerCoord + offset, int2(0, 0), int2(width - 1, height - 1));
            uint neighborID = gObjectTexture.Load(int3(sampleCoord, 0));
            
            bool isEdge = centerID != neighborID;
      
            difference.x += float(isEdge) * kPrewittHorizontalKernel[x][y];
            difference.y += float(isEdge) * kPrewittVerticalKernel[x][y];
            
        }

    }
    

    float weight = length(difference);
    weight = saturate(weight * gMaterial.lineWidth);
    
    //if (outlineMask > 0.75f)
    //{
    //    weight = 0.0f;
    //}
 
    //output.color.rgb = (1 - weight) * gTexture.Sample(gSampler, input.texcoord).rgb + weight *gMaterial.color;
    //This　is Lerp

    output.color.rgb = lerp(gTexture.Sample(gSampler, input.texcoord).rgb, gMaterial.color, weight);
    output.color.a = 1.0f;
    
    return output;
}