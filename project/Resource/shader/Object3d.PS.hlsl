#include "object3d.hlsli"
#include "Camera.hlsli"
#include"Light.hlsli"

//テクスチャを貼り付けたり、ライティングを行ったりと、もっとも主要なShaderである

struct Material
{
    float32_t4 color;
    int32_t lightType;
    float32_t4x4 uvTransform;
    float32_t shininess; //てかり
};

//ConstantBufferを定義する
//ConstantBuffer<構造体>変数名 : register(b0);//配置場所
//CPUから値を渡すにはConstantBufferという機能を利用する
ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<PointLight> gPointLight : register(b3);
Texture2D<float32_t4> gTexture : register(t2); //SRVはt
SamplerState gSampler : register(s0); //Samplerはs これを介してtextureを読む

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};


float GetCosin(float NdotL, int lightType)
{
    //コサインを求める
    return (lightType == 1) ? saturate(NdotL) : pow(NdotL * 0.5f + 0.5f, 2.0f);
}

float3 CalculatePointLightDiffuse(float3 normal, float3 worldPos, PointLight light, int lightType)
{
  
     //ポイントライトの方向を求める
    float3 toWorldPos = worldPos - light.position;
    //方向を求める
    float3 dir = normalize(toWorldPos);
    //距離を求める
    float distance = length(toWorldPos);
     //減衰率を求める
    float factor = pow(saturate(-distance / light.radius + 1.0f), light.decay);
    //内積を求める
    float NdotL = dot(normal, -dir);
    //コサインを求める
    float cos = GetCosin(NdotL, lightType);
    //カラーを求める    //ポイントライトのみの色
    return light.color.rgb * cos * light.intensity * factor;
    
}

float3 CalculatePointLightSpecular(float3 normal, float3 worldPos, float3 toEye, PointLight light, float shininess)
{
    //ポイントライトの方向を求める
    float3 toWorldPos = worldPos - light.position;
    //方向を求める
    float3 dir = normalize(toWorldPos);
    //反射
    //float3 reflect = reflect(dir, normal);
    //float NDotR = dot(normal, reflect);
    //float specularPow = pow(saturate(NDotR), shininess);
    float3 halfVector = normalize(-dir + toEye);
    float NDotH = dot(normal, halfVector);
    float spec = pow(saturate(NDotH), shininess);
    ////カラーを求める    //ポイントライトのみの色
    //return light.color.rgb * cos * light.intensity * factor;
    //色と後で付け足す
    return /*light.color.rgb **/spec * float3(1.0f, 1.0f, 1.0f); //反射色をここで設定
    
}


float3 CalculateDirectionalDiffuse(float3 normal, float3 dir, float3 color, float intensity, int lightType)
{
    float NdotL = dot(normal, -dir);
    float cos = GetCosin(NdotL, lightType);
    return color.rgb * cos * intensity; 
}

float3 CalculateDirectionalSpecular(float3 normal, float3 dir, float3 toEye, float3 color, float intensity, float shininess)
{
    float3 reflectLight = reflect(dir, normal);
    float3 halfVector = normalize(-dir + toEye);
    float NDotH = dot(normal, halfVector);
    float spec = pow(saturate(NDotH), shininess);
    return spec * float3(1.0f, 1.0f, 1.0f); //反射色をここで設定
}

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
    else
    {
  
        //法線情報 
        float32_t3 normalInput = normalize(input.normal);
        //ベースカラー
        float32_t3 baseColor = gMaterial.color.rgb * textureColor.rgb;
         //カメラに向かうベクトル
        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

        
        //ポイントライトについて
        float32_t3 PointLightDiffuse = CalculatePointLightDiffuse(normalInput, input.worldPosition, gPointLight, gMaterial.lightType);
        float32_t3 PointLightSpecular = CalculatePointLightSpecular(normalInput, input.worldPosition, toEye, gPointLight, gMaterial.shininess);

        float32_t3 DirectionalLightDiffuse = CalculateDirectionalDiffuse(normalInput, gDirectionalLight.direction, gDirectionalLight.color.rgb, gDirectionalLight.intensity, gMaterial.lightType);
        float32_t3 DirectionalLightSpecular = CalculateDirectionalSpecular(normalInput, gDirectionalLight.direction, toEye, gDirectionalLight.color.rgb, gDirectionalLight.intensity, gMaterial.shininess);
         
          //ディフューズ結果
        float32_t3 diffusegPointLight = baseColor * PointLightDiffuse;
        float32_t3 diffusegDirectionalLight = baseColor * DirectionalLightDiffuse;
            
        float32_t3 speculargPointLight = PointLightDiffuse * PointLightSpecular;
        float32_t3 speculargDirectionalLight = DirectionalLightDiffuse * DirectionalLightSpecular;
            
        output.color.rgb = diffusegDirectionalLight + speculargDirectionalLight + diffusegPointLight + speculargPointLight;
        //ライトモード共通
        output.color.a = gMaterial.color.a * textureColor.a;

    }

    //output.colorのαの値が0の時にPixelを棄却
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    return output;
}
