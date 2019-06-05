#pragma once
#include<vector>
#include<Windows.h>
#include<d3d11.h>
#include<d3dcompiler.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
#include"AppTools.h"
#include"GraphicMath.h"
using namespace GraphicMath;
using namespace DirectX;
namespace Utility
{
	struct  SimplePC
	{
		XMFLOAT4 Pos;
		XMFLOAT4 Color;
	};
	struct SimplePN
	{
		XMFLOAT4 Pos;
		XMFLOAT4 Dir;
	};
	class DirectBase
	{
	protected:
		IDXGISwapChain			*pSwapChain;
		ID3D11RenderTargetView	*pRenderTargetView;
		ID3D11Device			*pDevice;
		ID3D11DeviceContext		*pImmediateContext;
		
		D3D_DRIVER_TYPE			driverType;
		D3D_FEATURE_LEVEL		featureLevel;

		Tools::Window *win;
		HINSTANCE hInstance;
		//从hlsl中编译着色器
		virtual HRESULT  CompileShaderFromFile(wchar_t* fxFilename, char*EntryPoint, char*ShadeModel, ID3DBlob** ppBlobOut);
		//窗口的处理函数
		static LRESULT _stdcall WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		HRESULT setupSwapChain();
		virtual void MessageLoop();
		//消息处理函数
		virtual void MSGProcess(MSG& msg) = 0;
	public:
		DirectBase(HINSTANCE hinstance);
		//程序的初始化函数
		virtual void init();
		virtual void Render() = 0;
		virtual HRESULT InitDevice() = 0;
		virtual void Destory() = 0;
		virtual void run();
	};
	//2D图形
	namespace Figure2D
	{
		class Figure2D
		{
		protected:
			ID3D11Buffer			*pVertexBuffer;
			ID3D11VertexShader		*pVertexShader;
			ID3D11PixelShader		*pPixelShader;	
			ID3D11InputLayout		*pInputLayout2D;

			ID3D11Device			*ppDevice;
			ID3D11DeviceContext		*ppImmediateContext;
			UINT					vertexnum;
		public:
			virtual void Setup() = 0;
			virtual void PushToSwapChain() = 0;
			virtual void Release() = 0;
		};
		class Points :public Figure2D
		{
			SimplePC*  buffer;
			UINT size;//内存长度
		public:
			Points(ID3D11Device*pd3dDevice, ID3D11DeviceContext*pContext, 
				ID3D11PixelShader*pd3dPixelShader, ID3D11VertexShader*pd3dVertexShader,ID3D11InputLayout*pInput);
			void Setup();
			void PushToSwapChain();
			void addapoint(XMFLOAT2 position, XMFLOAT4 Color);
			void clear();
			void Release();
		};
	}
	//3D图形
	namespace Figure3D
	{
		//3D图形基类	
		class Figure3D
		{
		protected:
			ID3D11Buffer			*pVertexBuffer;
			ID3D11Buffer			*pConstBuffer;

			ID3D11VertexShader		*pVertexShader;
			ID3D11PixelShader		*pPixelShader;
			ID3D11InputLayout		*pInputLayout3D;

			ID3D11Device			*pDevice;
			ID3D11DeviceContext		*pImmediateContext;
			UINT					vertexnum;
		public:
			Figure3D(ID3D11VertexShader*pVertex, ID3D11PixelShader*pPixel, ID3D11Device*pDevice, ID3D11DeviceContext*pImmediate,ID3D11InputLayout*pInput);
			virtual void SetupBuffer() = 0;
			virtual void PushToSwapChain() = 0;
			virtual void  Release() = 0;
		};
		class Cylinder
			:public Figure3D
		{
		private:
			ConstBuffer* cb;
			SimplePN *buffer;
		public:
			Cylinder(ID3D11Device*pDev, ID3D11DeviceContext*pCon, ID3D11PixelShader*pPixel, ID3D11VertexShader*pVertex,ID3D11InputLayout *pInput);
			void SetupBuffer();
			UINT generate(UINT N,float r);
			UINT generate(UINT N, float rtop, float rbottom,float h);
			void SetupConstantBuffer(ConstBuffer*cb);
			void PushToSwapChain();
			void UpdateConstBuffer(ConstBuffer*cb);
			void Release();
		};
	}
}


