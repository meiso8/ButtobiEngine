#include "object3d.hlsli"
#include "Camera.hlsli"
#include"Light.hlsli"

//テクスチャを貼り付けたり、ライティングを行ったりと、もっとも主要なShaderである

struct Material
{
    float32_t4 color;
    int32_t lightType;
    float32_t4x4 uvTransform;
    float32_t shininess;//てかり
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
  
        float32_t3 diffusegDirectionalLight;
        float32_t3 diffusegPointLight;
        
        //ライトのコサイン
        float pointLightCos;
        float directionalLightCos;
        
        //ポイントライトと方向ライトのカラーを求める
        float32_t3 pointLightColor;
        float32_t3 directionalLightColor;
      
        //ポイントライトの方向を求める
        float32_t3 pointLightDirection = normalize(input.worldPosition - gPointLight.position);
        
         //ポイントライトの減衰率を求める
        float pointLightDistance = length(input.worldPosition - gPointLight.position);
        //float factor = 1.0f / (pointLightDistance * pointLightDistance);
        float factor = pow(saturate(-pointLightDistance / gPointLight.radius + 1.0f), gPointLight.decay);
        
        if (gMaterial.lightType == 1)
        {
            
            
          //ライトのコサイン
            directionalLightCos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
            pointLightCos = saturate(dot(normalize(input.normal), -pointLightDirection));
        
        //ポイントライトと方向ライトのカラーを求める
            pointLightColor = gPointLight.color.rgb * pointLightCos * gPointLight.intensity*factor;
            directionalLightColor = gDirectionalLight.color.rgb * directionalLightCos * gDirectionalLight.intensity;
            
            //カメラに向かうベクトル
            float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
            
            //ポイントライトの設定
            float32_t3 reflectPointLight = reflect(pointLightDirection, normalize(input.normal));
            float NDotLPointLight = dot(normalize(input.normal), reflectPointLight);
            float specularPowPointLight = pow(saturate(NDotLPointLight), gMaterial.shininess);

           
            float32_t3 reflectDirectionalLight = reflect(gDirectionalLight.direction, normalize(input.normal));
            float32_t3 halfDirectionalLightVector = normalize(-gDirectionalLight.direction + toEye);
            float NDotHDirectionalLight = dot(normalize(input.normal), halfDirectionalLightVector);
        //float RdotE = dot(reflectLight, toEye);
            float specularPowDirectionalLight = pow(saturate(NDotHDirectionalLight), gMaterial.shininess);

            diffusegPointLight = gMaterial.color.rgb * textureColor.rgb * pointLightColor;
            diffusegDirectionalLight = gMaterial.color.rgb * textureColor.rgb * directionalLightColor;
            
            float32_t3 speculargPointLight =
        pointLightColor * specularPowPointLight * float32_t3(1.0f, 1.0f, 1.0f); //反射色をここで設定


    
            float32_t3 speculargDirectionalLight =
      directionalLightColor * specularPowDirectionalLight * float32_t3(1.0f, 1.0f, 1.0f); //反射色をここで設定

     
            output.color.rgb = diffusegDirectionalLight + speculargDirectionalLight + diffusegPointLight + speculargPointLight;
      
        }
        else if (gMaterial.lightType == 2)
        {
         //half lambert
        
        //法線とライトの方向の内積
            float NdotDirectionalLight = dot(normalize(input.normal), -gDirectionalLight.direction);
            float NdotPointLight = dot(normalize(input.normal), -pointLightDirection);
           // コサインを求める
            directionalLightCos = pow(NdotDirectionalLight * 0.5f + 0.5f, 2.0f);
            pointLightCos = pow(NdotPointLight * 0.5f + 0.5f, 2.0f);

            //ポイントライトと方向ライトのカラーを求める
            pointLightColor = gPointLight.color.rgb * pointLightCos * gPointLight.intensity*factor;
            directionalLightColor = gDirectionalLight.color.rgb * directionalLightCos * gDirectionalLight.intensity;
             //diffuseを求める
            diffusegPointLight = gMaterial.color.rgb * textureColor.rgb * pointLightColor;
            diffusegDirectionalLight = gMaterial.color.rgb * textureColor.rgb * directionalLightColor;

            output.color.rgb = diffusegPointLight + diffusegDirectionalLight;

        }
        
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
