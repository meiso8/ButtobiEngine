#include "Particle.hlsli"

//テクスチャを貼り付けたり、ライティングを行ったりと、もっとも主要なShaderである
struct Material
{
    float32_t4 color;
    int32_t lightType;
    float32_t4x4 uvTransform;

};

//ConstantBufferを定義する
//ConstantBuffer<構造体>変数名 : register(b0);//配置場所
//CPUから値を渡すにはConstantBufferという機能を利用する
ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float32_t4> gTexture : register(t2); //SRVはt
SamplerState gSampler : register(s0); //Samplerはs これを介してtextureを読む

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};


float3 ApplySRGBGamma(float3 linearColor)
{
    return linearColor < 0.0031308 ? 12.92 * linearColor : 1.055 * pow(linearColor, 1.0 / 2.4) - 0.055;
}


PixelShaderOutput main(VertexShaderOutput input)
{
 
    PixelShaderOutput output;
    
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    output.color = gMaterial.color * textureColor * input.color; //ベクトル*ベクトルと記述すると乗算が行われる
    
    //色を調整する
    output.color.rgb = ApplySRGBGamma(output.color.rgb);
    
    //textureのα値ガ0.0以下の時にpixleを棄却
    if (textureColor.a == 0.0)
    {
        discard;
    }
    
    return output;
}
