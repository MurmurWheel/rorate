//用于 2D的像素着色器输入
struct PS_INPUT
{
    float4 Pos : SV_Position;
    float4 Color : COLOR0;
};
//用于3D的像素着色器输入
struct PS_INPUT3D
{
    float4 Pos : SV_Position;
    float4 Norm : COLOR1;
};
cbuffer ConstBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Project;
    float4 LightDir; //光的方向
    float4 LightColor; //光的颜色
    float4 EnviromentLight; //环境光
}
//2D像素着色程序
float4 PS_2D(PS_INPUT input):SV_Target
{
    return input.Color;
}
//3D像素着色程序
float4 PS_3D(PS_INPUT3D input):SV_Target
{
    float4 finalcolor;
    finalcolor = dot(LightDir, input.Norm) * LightColor;
    finalcolor.a = 1.0f;
    finalcolor = finalcolor + EnviromentLight;
    finalcolor = saturate(finalcolor);
    return finalcolor;
}