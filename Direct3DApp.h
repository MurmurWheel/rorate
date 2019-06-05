#pragma once
#include "Utility.h"
class Direct3DApp :
	public Utility::DirectBase
{
private:
	void MessageLoop();
	//消息处理函数
	void MSGProcess(MSG&msg) {}
	ID3D11VertexShader			*pVertex2D;
	ID3D11PixelShader			*pPixel2D;
	ID3D11PixelShader			*pPixel3D;
	ID3D11VertexShader			*pVertex3D;
	
	ID3D11InputLayout			*pInput3D;
	ID3D11InputLayout			*pInput2D;

	void SetupVertexShader2D();
	void SetupPixelShader2D();
	void SetupVertexShader3D();
	void SetupPixelShader3D();
	//窗口消息处理函数
	static LRESULT _stdcall WndProcWindow(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void mouse(UINT message,UINT x,UINT y);
	//创建一个窗口
	void initWindow();
	//时间
	Tools::Timer *ti;
	//几何构建
	Utility::Figure2D::Points *point1,*point2,*point3,*point4;
	Utility::Figure3D::Cylinder *cylinder1,*cylinder2,*cylinder3,*cylinder4;
	//Const Buffer
	GraphicMath::CB cb;
public:
	Direct3DApp(HINSTANCE hinstance);
	void run();//运行程序
	HRESULT InitDevice();//初始化设备和数据
	void Destory() {}
	void Render();
	~Direct3DApp();
};

