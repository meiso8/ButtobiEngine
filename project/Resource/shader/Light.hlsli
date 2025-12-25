struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

struct PointLight
{
    float32_t4 color; 
    float32_t3 position; 
    float intensity; 
    float radius; 
    float decay; 
    float padding[2];
};

struct SpotLight
{
    float32_t4 color; 
    float32_t3 position; 
    float32_t intensity; 
    float32_t3 direction;
    float32_t distance;
    float32_t decay;
    float32_t cosAngle; 
    float32_t cosFalloffStart;
    float padding[1];
};