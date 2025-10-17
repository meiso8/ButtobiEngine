#include "Sprite.hlsli"

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

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    
     //textureのα値ガ0.0以下の時にpixleを棄却
    if (textureColor.a == 0.0)
    {
        discard;
    }
    
    output.color = gMaterial.color * textureColor; //ベクトル*ベクトルと記述すると乗算が行われる
    float3 targetTint = float3(0.85, 0.9, 1.0); // 赤と緑を少し抑えて青みを足す
    output.color.rgb *= targetTint;
    
    float3 color = output.color.rgb;
    //// トーン調整スケール
    //float3 toneScale = float3(0.95f, 0.98f, 0.98f);

    //// 色補正
    //color *= toneScale;

    // 彩度調整（オプション）
    float gray = dot(color, float3(0.299f, 0.587f, 0.114f));
    float saturation = 1.0f; // 少し鮮やかに
    color = lerp(float3(gray, gray, gray), color, saturation);

    //// 最終色
    //output.color.rgb = saturate(color);
      
    return output;
}
