struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct PointLight
{
    float4 color;
    float3 position; 
    float intensity;
    float radius; 
    float decay;
    float padding[2];
};

struct SpotLight
{
    float4 color; 
    float3 position; 
    float intensity; 
    float3 direction;
    float distance;
    float decay;
    float cosAngle;
    float cosFalloffStart;
    float padding[1];
};
