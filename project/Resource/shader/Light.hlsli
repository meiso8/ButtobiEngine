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
