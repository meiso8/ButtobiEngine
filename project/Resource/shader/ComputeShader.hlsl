//[numthreads(1, 1, 1)]
//void main( uint3 DTid : SV_DispatchThreadID )
//{
//}

cbuffer ConstantBuffer : register(b0)
{
    float time;
};

RWStructuredBuffer<float3> vertices : register(u0); 

[numthreads(256, 1, 1)] 
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    float3 currentVertex = vertices[dispatchThreadID.x];


    currentVertex.y += sin(time + dispatchThreadID.x) * 0.5f;

    vertices[dispatchThreadID.x] = currentVertex;
}