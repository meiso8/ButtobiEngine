#include "object3d.hlsli"
#include "Camera.hlsli"
#include"DirectionalLight.hlsli"

//テクスチャを貼り付けたり、ライティングを行ったりと、もっとも主要なShaderである


struct Material
{
    float32_t4 color;
    int32_t lightType;
    float32_t4x4 uvTransform;
    float32_t shininess;
};

//ConstantBufferを定義する
//ConstantBuffer<構造体>変数名 : register(b0);//配置場所
//CPUから値を渡すにはConstantBufferという機能を利用する
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);

Texture2D<float32_t4> gTexture : register(t2); //SRVはt
SamplerState gSampler : register(s0); //Samplerはs これを介してtextureを読む

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};


PixelShaderOutput main(VertexShaderOutput input)
{
 
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    //textureのα値ガ0.2以下の時にpixleを棄却
    if (textureColor.a <= 0.2)
    {
        discard;
    }
    

    PixelShaderOutput output;

    if (gMaterial.lightType == 0)
    {
        //Lightingしない場合。前回までと同じ演算
        output.color = gMaterial.color * textureColor; //ベクトル*ベクトルと記述すると乗算が行われる
     
    }
    else if (gMaterial.lightType == 1)
    {
        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        float32_t3 reflectLight = reflect(gDirectionalLight.direction, normalize(input.normal));
    
        float32_t3 halfVector = normalize(-gDirectionalLight.direction + toEye);
        float NDotH = dot(normalize(input.normal), halfVector);
        
        //float RdotE = dot(reflectLight, toEye);
        float specularPow = pow(saturate(NDotH), gMaterial.shininess);

        
        
        float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
           
        float32_t3 diffuse =
        gMaterial.color.rbg * textureColor.rgb * gDirectionalLight.color.rbg * cos * gDirectionalLight.intensity;
    
        float32_t3 specular =
        gDirectionalLight.color.rgb * gDirectionalLight.intensity * specularPow * float32_t3(1.0f, 1.0f, 1.0f); //反射色をここで設定

        

        output.color.rgb = diffuse + specular;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    else
    {
        //half lambert
        
        //法線とライトの方向の内積
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
         
        output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        output.color.a = gMaterial.color.a * textureColor.a;
    }
    

    //output.colorのαの値が0の時にPixelを棄却
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    return output;
}
