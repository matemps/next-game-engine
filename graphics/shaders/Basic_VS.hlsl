#include "Common.hlsli"

cbuffer WVP
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

PSInput VSMain(VSInput input)
{
    PSInput output;

    float4 pos = float4(input.position, 1);

    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Projection);

    output.position = pos;
    output.color = input.color;

    return output;
}