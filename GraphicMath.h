#pragma once
#include<DirectXMath.h>
#include<string>
namespace GraphicMath
{
	using namespace DirectX;
	struct ConstBuffer
	{
		XMMATRIX World;
		XMMATRIX View;
		XMMATRIX Project;
		XMFLOAT4 LightDir;//光方向
		XMFLOAT4 LightColor;//光的颜色
		XMFLOAT4 EnviromentLight;//环境光
	};
	class CB
	{
	private:
		ConstBuffer*cb;
	public:
		CB();
		void SetLIghtDir(XMFLOAT3 LightDir);
		void SetLightColor(XMFLOAT4 LightColor);
		void SetEniromentLight(XMFLOAT4 EnvLight);
		void SetWorld(XMMATRIX& World);
		void SetView(XMMATRIX& View);
		void SetProject(XMMATRIX& Project);
		ConstBuffer* GetStr();
		~CB();
	};
	XMVECTOR Normal(XMVECTOR& vertor);
	void print(XMMATRIX& matrix);
	void print(XMVECTOR& vector);
	void print(XMFLOAT3& point);
	void print(std::string note, XMVECTOR& vector);
	void print(std::string note, XMMATRIX& xmatrix);
};

