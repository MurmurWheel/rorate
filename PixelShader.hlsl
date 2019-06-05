//���� 2D��������ɫ������
struct PS_INPUT
{
    float4 Pos : SV_Position;
    float4 Color : COLOR0;
};
//����3D��������ɫ������
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
    float4 LightDir; //��ķ���
    float4 LightColor; //�����ɫ
    float4 EnviromentLight; //������
}
//2D������ɫ����
float4 PS_2D(PS_INPUT input):SV_Target
{
    return input.Color;
}
//3D������ɫ����
float4 PS_3D(PS_INPUT3D input):SV_Target
{
    float4 finalcolor;
    finalcolor = dot(LightDir, input.Norm) * LightColor;
    finalcolor.a = 1.0f;
    finalcolor = finalcolor + EnviromentLight;
    finalcolor = saturate(finalcolor);
    return finalcolor;
}