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
};