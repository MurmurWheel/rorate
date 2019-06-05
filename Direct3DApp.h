#pragma once
#include "Utility.h"
class Direct3DApp :
	public Utility::DirectBase
{
private:
	void MessageLoop();
	//��Ϣ������
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
	//������Ϣ������
	static LRESULT _stdcall WndProcWindow(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	void mouse(UINT message,UINT x,UINT y);
	//����һ������
	void initWindow();
	//ʱ��
	Tools::Timer *ti;
	//���ι���
	Utility::Figure2D::Points *point1,*point2,*point3,*point4;
	Utility::Figure3D::Cylinder *cylinder1,*cylinder2,*cylinder3,*cylinder4;
	//Const Buffer
	GraphicMath::CB cb;
public:
	Direct3DApp(HINSTANCE hinstance);
	void run();//���г���
	HRESULT InitDevice();//��ʼ���豸������
	void Destory() {}
	void Render();
	~Direct3DApp();
};

