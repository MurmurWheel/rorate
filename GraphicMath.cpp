#include "GraphicMath.h"
#include<stdio.h>
namespace GraphicMath
{
	CB::CB()
		:cb(new ConstBuffer())
	{
	}

	void CB::SetLIghtDir(XMFLOAT3 LightDir)
	{
		cb->LightDir.x = LightDir.x;
		cb->LightDir.y = LightDir.y;
		cb->LightDir.z = LightDir.z;
		cb->LightDir.w = 0.0f;

	}

	void CB::SetLightColor(XMFLOAT4 LightColor)
	{
		cb->LightColor = LightColor;
	}

	void CB::SetEniromentLight(XMFLOAT4 EnvLight)
	{
		cb->EnviromentLight = EnvLight;
	}

	void CB::SetWorld(XMMATRIX & World)
	{
		cb->World = World;
	}

	void CB::SetView(XMMATRIX & View)
	{
		cb->View = View;
	}

	void CB::SetProject(XMMATRIX & Projetc)
	{
		cb->Project = Projetc;
	}

	ConstBuffer * CB::GetStr()
	{
		return cb;
	}

	CB::~CB()
	{
		delete cb;
	}

	XMVECTOR Normal(XMVECTOR & vertor)
	{
		float x = vertor.m128_f32[0] * vertor.m128_f32[0] + vertor.m128_f32[1] * vertor.m128_f32[1] + 
			vertor.m128_f32[2] * vertor.m128_f32[2] + vertor.m128_f32[3] * vertor.m128_f32[3];
		x = sqrtf(x);
		return XMVectorSet(vertor.m128_f32[0]/x, vertor.m128_f32[1] / x,vertor.m128_f32[2] / x,vertor.m128_f32[3] / x);
	}
	void print(XMMATRIX& matrix)
	{

		printf("matrix-start:\n");
		printf("(%-12f%-12f%-12f%-12f\n", matrix.r[0].m128_f32[0], matrix.r[0].m128_f32[1], matrix.r[0].m128_f32[2], matrix.r[0].m128_f32[3]);
		printf("(%-12f%-12f%-12f%-12f\n", matrix.r[1].m128_f32[0], matrix.r[1].m128_f32[1], matrix.r[1].m128_f32[2], matrix.r[1].m128_f32[3]);
		printf("(%-12f%-12f%-12f%-12f\n", matrix.r[2].m128_f32[0], matrix.r[2].m128_f32[1], matrix.r[2].m128_f32[2], matrix.r[2].m128_f32[3]);
		printf("(%-12f%-12f%-12f%-12f\n", matrix.r[3].m128_f32[0], matrix.r[3].m128_f32[1], matrix.r[3].m128_f32[2], matrix.r[3].m128_f32[3]);
		printf("matrix-end\n");
	}
	void print(XMVECTOR& vector)
	{
		printf("vector-start\n");
		printf("%-12f%-12f%-12f%-12f\n", vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2], vector.m128_f32[3]);
		printf("vector-end\n");
	}
	void print(XMFLOAT3& point)
	{
		printf("xmfloat3-start:\n");
		printf("%-12f%-12f%-12f\n", point.x, point.y, point.z);
		printf("xmfloat3-end");
	}
	void print(std::string note, XMVECTOR & vector)
	{
		printf(note.c_str());
		printf("%-12f%-12f%-12f%-12f\n", vector.m128_f32[0], vector.m128_f32[1], vector.m128_f32[2], vector.m128_f32[3]);
	}
	void print(std::string note, XMMATRIX & matrix)
	{
		printf(note.c_str());
		printf("(%-12f%-12f%-12f%-12f\n", matrix.r[0].m128_f32[0], matrix.r[0].m128_f32[1], matrix.r[0].m128_f32[2], matrix.r[0].m128_f32[3]);
		printf("(%-12f%-12f%-12f%-12f\n", matrix.r[1].m128_f32[0], matrix.r[1].m128_f32[1], matrix.r[1].m128_f32[2], matrix.r[1].m128_f32[3]);
		printf("(%-12f%-12f%-12f%-12f\n", matrix.r[2].m128_f32[0], matrix.r[2].m128_f32[1], matrix.r[2].m128_f32[2], matrix.r[2].m128_f32[3]);
		printf("(%-12f%-12f%-12f%-12f\n", matrix.r[3].m128_f32[0], matrix.r[3].m128_f32[1], matrix.r[3].m128_f32[2], matrix.r[3].m128_f32[3]);
	}
};
