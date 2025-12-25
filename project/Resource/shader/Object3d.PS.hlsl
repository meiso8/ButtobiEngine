#include "object3d.hlsli"
#include "Camera.hlsli"
#include"Light.hlsli"


struct Material
{
    float32_t4 color;
    int32_t lightType;
    float32_t4x4 uvTransform;
    float32_t shininess;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<SpotLight> gSpotLight : register(b3);

Texture2D<float32_t4> gTexture : register(t2); 
SamplerState gSampler : register(s0); 

StructuredBuffer<PointLight> gPointLights : register(t4);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};


float GetCosin(float NdotL, int lightType)
{
    return (lightType == 1) ? saturate(NdotL) : pow(NdotL * 0.5f + 0.5f, 2.0f);
}

float3 CalculatePointLightDiffuse(float3 normal, float3 worldPos, PointLight light, int lightType)
{
  
    float3 toWorldPos = worldPos - light.position;
    float3 dir = normalize(toWorldPos);
    float distance = length(toWorldPos);
    float factor = pow(saturate(-distance / light.radius + 1.0f), light.decay);
    float NdotL = dot(normal, -dir);
    float cos = GetCosin(NdotL, lightType);
    return light.color.rgb * cos * light.intensity * factor;
    
}


float3 CalculateSpotLightDiffuse(float3 normal, float3 worldPos, SpotLight light, int lightType)
{
  
    float3 toWorldPos = worldPos - light.position;
    float3 dir = normalize(toWorldPos);
    float cosAngle = dot(dir, normalize(light.direction));
    float falloffFactor = saturate((cosAngle - light.cosAngle) / (light.cosFalloffStart - light.cosAngle));
    float attenuationFactor = pow(saturate(light.distance / 1.0f), light.decay);
    return light.color.rgb * light.intensity * falloffFactor * attenuationFactor;
    
}

float3 CalculatePointLightSpecular(float3 normal, float3 worldPos, float3 toEye, PointLight light, float shininess)
{
    float3 toWorldPos = worldPos - light.position;
    float3 dir = normalize(toWorldPos);
    float3 halfVector = normalize(-dir + toEye);
    float NDotH = dot(normal, halfVector);
    float spec = pow(saturate(NDotH), shininess);
    return spec * light.color.rgb;//<-selectReflectColor
    
}


float3 CalculateDirectionalDiffuse(float3 normal, float3 dir, float3 color, float intensity, int lightType)
{
    float NdotL = dot(normal, -dir);
    float cos = GetCosin(NdotL, lightType);
    return color.rgb * cos * intensity;
}

float3 CalculateDirectionalSpecular(float3 normal, float3 dir, float3 toEye, float3 color, float shininess)
{
    float3 reflectLight = reflect(dir, normal);
    float3 halfVector = normalize(-dir + toEye);
    float NDotH = dot(normal, halfVector);
    float spec = pow(saturate(NDotH), shininess);
    return spec * color; //<-selectReflectColor
}

PixelShaderOutput main(VertexShaderOutput input)
{
 
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    

    if (textureColor.a <= 0.5)
    {
        discard;
    }
    
    PixelShaderOutput output;

    if (gMaterial.lightType == 0)
    {
        output.color = gMaterial.color * textureColor; 
     
    }
    else
    {
 
        // ==========================//Common//====================================
        //normal
        float32_t3 normalInput = normalize(input.normal);
        //baseColor
        float32_t3 baseColor = gMaterial.color.rgb * textureColor.rgb;
         //toCameraVector
        float32_t3 toEye = normalize(gCamera.worldPosition - input.worldPosition);
        // ======================================================================
  
        // totalPointLightDiffuse
        float3 pointLightTotalDiffuse = float3(0, 0, 0);
        float3 pointLightTotalSpecular = float3(0, 0, 0);

        [loop]
        for (int i = 0; i < 20; ++i)
        {
            pointLightTotalDiffuse += CalculatePointLightDiffuse(normalInput, input.worldPosition, gPointLights[i], gMaterial.lightType);
        }
        
        float32_t3 DirectionalLightDiffuse = CalculateDirectionalDiffuse(normalInput, gDirectionalLight.direction, gDirectionalLight.color.rgb, gDirectionalLight.intensity, gMaterial.lightType);

        
        float32_t3 spotLightDiffuse = CalculateSpotLightDiffuse(normalInput, input.worldPosition, gSpotLight, gMaterial.lightType);

        if (gMaterial.lightType == 1)
        {
            
            [loop]
            for (int i = 0; i < 20; ++i)
            {
                pointLightTotalSpecular += CalculatePointLightSpecular(normalInput, input.worldPosition, toEye, gPointLights[i], gMaterial.shininess);
            }
        
            //directionalLightReflect
            float32_t3 speculargDirectionalLight =
            DirectionalLightDiffuse *
            CalculateDirectionalSpecular(normalInput, gDirectionalLight.direction, toEye, gDirectionalLight.color.rgb, gMaterial.shininess);
         
            output.color.rgb =
            baseColor * (DirectionalLightDiffuse + pointLightTotalDiffuse + spotLightDiffuse) +
            (speculargDirectionalLight + pointLightTotalSpecular);
            
        }
        else
        {
            //NoneReflect
            output.color.rgb = baseColor * (DirectionalLightDiffuse + pointLightTotalDiffuse + spotLightDiffuse);

        }
        
          //commonLightMode
        output.color.a = gMaterial.color.a * textureColor.a;
    }

    //output.color discard
    if (output.color.a == 0.0)
    {
        discard;
    }
    
    return output;
}
