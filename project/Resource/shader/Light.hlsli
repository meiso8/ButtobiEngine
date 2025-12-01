struct DirectionalLight
{
    float32_t4 color;
    float32_t3 direction;
    float intensity;
};

struct PointLight
{
    float32_t4 color; //色
    float32_t3 position; //位置
    float intensity; //高度
    float radius; //半径
    float decay; //減衰率
    float padding[2];
};

struct SpotLight
{
    float32_t4 color; //色
    float32_t3 position; //位置
    float32_t intensity; //輝度
    float32_t3 direction; //スポットライトの方向
    float32_t distance; //ライトの届く範囲
    float32_t decay; //減衰率
    float32_t cosAngle; //スポットライトの余弦
};