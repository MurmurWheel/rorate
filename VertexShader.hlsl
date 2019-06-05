struct PS_INPUT
{
    float4 Pos : SV_Position;
    float4 Color : COLOR0;
};
struct VS_INPUT
{
    float4 Pos : POSITIONT;
    float4 Color : COLOR;
};

cbuffer ConstBuffer:register(b0)
{
    matrix World;
    matrix View;
    matrix Project;
    float4 LightDir;//光的方向
    float4 LightColor;//光的颜色
    float4 EnviromentLight;//环境光
}

//用来实现2D的绘图功能
PS_INPUT VS_2D(VS_INPUT input)
{
    VS_INPUT output;
    output.Color = input.Color;
    output.Pos = input.Pos;
    return output;
}
struct VS_INPUT3D
{
    float4 Pos : POSITION;
    float4 Dir : NORMAL;
};
struct PS_INPUT3D
{
    float4 Pos : SV_Position;
    float4 Norm : COLOR1;
};
PS_INPUT3D VS_3D(VS_INPUT3D input)
{
    PS_INPUT3D output;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Project);
    output.Norm = mul(input.Dir, World);
    
    
    return output;
}