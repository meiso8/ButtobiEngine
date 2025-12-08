#include "Sprite.hlsli"

//テクスチャを貼り付けたり、ライティングを行ったりと、もっとも主要なShaderである
struct Material
{
    float32_t4 color;
    int32_t lightType;
    float32_t4x4 uvTransform;

};


ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t2); 
SamplerState gSampler : register(s0); 

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    

    if (textureColor.a <= 0.01)
    {
        discard;
    }
    
    
    //float3 color = { 60.0f / 255.0f, 40.0f / 255.0f, 40.0f / 255.0f };
    
    //if (textureColor.r > 0.5f){
    //    textureColor.r = 1.0f - 2.0f * (1.0f - textureColor.r) * (1.0f - color.r);
    //} else {
    //    textureColor.r = 2.0f * textureColor.r * color.r;
    //}
    
    //if (textureColor.g > 0.5f)
    //{
    //    textureColor.g = 1.0f - 2.0f * (1.0f - textureColor.g) * (1.0f - color.g);
    //}
    //else
    //{
    //    textureColor.g = 2.0f * textureColor.g * color.g;
    //}
    //if (textureColor.b > 0.5f)
    //{
    //    textureColor.b = 1.0f - 2.0f * (1.0f - textureColor.b) * (1 - color.b);
    //}
    //else
    //{
    //    textureColor.b = 2.0f * textureColor.b * color.b;
    //}
      
    output.color = gMaterial.color * textureColor;

    return output;
}
