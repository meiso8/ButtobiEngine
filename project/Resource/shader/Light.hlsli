struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

struct PointLight
{
    float32_t4 color; // Color
    float32_t3 position; // Position
    float intensity; // Intensity
    float radius; // Radius
    float decay; // Decay rate
    float padding[2];
};

struct SpotLight
{
    float32_t4 color; // Color
    float32_t3 position; // Position
    float32_t intensity; // Intensity
    float32_t3 direction; // Direction of spotlight
    float32_t distance; // Range of the light
    float32_t decay; // Decay rate
    float32_t cosAngle; // Cosine of spotlight angle
    float padding[2];
};