#include "Direct3DApp.h"



void Direct3DApp::MessageLoop()
{
	MSG msg = { 0 };
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			MSGProcess(msg);
		}
		else
		{
			Render();
		}
	}
}



void Direct3DApp::SetupVertexShader2D()
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;
	hr = CompileShaderFromFile(L"VertexShader.hlsl", "VS_2D", "vs_4_0", &pBlob);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Compile Vertex Shader Error!", L"ERROR", MB_OK);
		abort();
		return;
	}
	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pVertex2D);
	if (FAILED(hr)) {
		pBlob->Release();
		return;
	}
	D3D11_INPUT_ELEMENT_DESC inputLayout[]=
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0}
	};
	UINT numElements = ARRAYSIZE(inputLayout);
	hr = pDevice->CreateInputLayout(inputLayout, numElements, pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), &pInput2D);
	pBlob->Release();
	if (FAILED(hr))
		return;
}

void Direct3DApp::SetupPixelShader2D()
{
	ID3DBlob *pBlob = 0;
	HRESULT hr = S_OK;
	hr=CompileShaderFromFile(L"PixelShader.hlsl", "PS_2D", "ps_4_0", &pBlob);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Compile Pixel Failes", L"ERROR", MB_OK);
		abort();
		return;
	}
	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pPixel2D);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Create Pixel Shader Failed!", L"ERROR", MB_OK);
		abort();
	}
}

void Direct3DApp::SetupPixelShader3D()
{
	ID3DBlob *pBlob = 0;
	HRESULT hr = S_OK;
	hr = CompileShaderFromFile(L"PixelShader.hlsl", "PS_3D", "ps_4_0", &pBlob);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Compile Pixel Failes", L"ERROR", MB_OK);
		abort();
		return;
	}
	hr = pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pPixel3D);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Create Pixel Shader Failed!", L"ERROR", MB_OK);
		abort();
	}
}

void Direct3DApp::SetupVertexShader3D()
{
	HRESULT hr = S_OK;
	ID3DBlob* pBlob = NULL;
	hr = CompileShaderFromFile(L"VertexShader.hlsl", "VS_3D", "vs_4_0", &pBlob);
	if (FAILED(hr)) {
		MessageBox(NULL, L"Compile Vertex Shader Error!", L"ERROR", MB_OK);
		abort();
		return;
	}
	hr = pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), NULL, &pVertex3D);
	if (FAILED(hr)) {
		pBlob->Release();
		return;
	}
	D3D11_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	
	UINT numElements = ARRAYSIZE(inputLayout);
	hr = pDevice->CreateInputLayout(inputLayout, numElements, pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), &pInput3D);
	pBlob->Release();
	if (FAILED(hr))
		return;
}

LRESULT Direct3DApp::WndProcWindow(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	case WM_QUIT:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

void Direct3DApp::mouse(UINT message, UINT x, UINT y)
{
	
}

Direct3DApp::Direct3DApp(HINSTANCE hinstance)
	:DirectBase(hinstance),cb()
{
}

void Direct3DApp::initWindow()
{
	win->setWndProc(WndProcWindow);
	win->setInitPos(0, 0);
	win->setWindowSize(1920, 1080);
	win->regisClass(L"WINDOW");
	win->setWindowName(L"Direct3D 11 Window");
	win->createWindow(L"WINDOW", Tools::WS_STYLE::ws_overlappedwindow);
	win->displayWindow();
}

void Direct3DApp::run()
{
	initWindow();
	InitDevice();
	MessageLoop();
}

HRESULT Direct3DApp::InitDevice()
{

	this->setupSwapChain();
	SetupVertexShader2D();
	SetupPixelShader2D();
	SetupPixelShader3D();
	SetupVertexShader3D();
	

	ti = new Tools::Timer();
	ti->SetStartPoint();

	point1 = new Utility::Figure2D::Points(pDevice, pImmediateContext, pPixel2D, pVertex2D, pInput2D);
	point2 = new Utility::Figure2D::Points(pDevice, pImmediateContext, pPixel2D, pVertex2D, pInput2D);
	point3 = new Utility::Figure2D::Points(pDevice, pImmediateContext, pPixel2D, pVertex2D, pInput2D);
	point4 = new Utility::Figure2D::Points(pDevice, pImmediateContext, pPixel2D, pVertex2D, pInput2D);
	float x, y;
	for (int i = 0; i < 200000; i ++)
	{
		x = rand() % 1000 / 1000.0f;
		y = rand() % 1000 / 1000.0f;
		point1->addapoint(XMFLOAT2(x, y), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
		point2->addapoint(XMFLOAT2(x, -y), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
		point3->addapoint(XMFLOAT2(-x, -y), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));
		point4->addapoint(XMFLOAT2(-x, y), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f));
	}
	point1->Setup();
	point2->Setup();
	point3->Setup();
	point4->Setup();

	cylinder1 = new Utility::Figure3D::Cylinder(pDevice, pImmediateContext, pPixel3D, pVertex3D, pInput3D);
	cylinder2 = new Utility::Figure3D::Cylinder(pDevice, pImmediateContext, pPixel3D, pVertex3D, pInput3D);
	cylinder3 = new Utility::Figure3D::Cylinder(pDevice, pImmediateContext, pPixel3D, pVertex3D, pInput3D);
	cylinder4 = new Utility::Figure3D::Cylinder(pDevice, pImmediateContext, pPixel3D, pVertex3D,pInput3D);
	cylinder1->generate(3000,0.3f, 0.3f, 0.5);
	cylinder2->generate(3000, 1.0f, 1.0f, 2.0);
	cylinder3->generate(3000, 1.0f, 1.0f, 2.0);
	cylinder4->generate(3000, 1.0f, 1.0f, 2.0);

	XMMATRIX World, View, Project;
	
	World = XMMatrixIdentity();
	World = XMMatrixTranspose(World);
	cb.SetWorld(World);
	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, 100.0f, 1.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	View = XMMatrixLookAtLH(Eye, At, Up);
	View = XMMatrixTranspose(View);
	cb.SetView(View);
	Project = XMMatrixPerspectiveFovLH(XM_PIDIV4/10, (float)win->getWidth() / (float)win->getHeight(), 90.0f, 120.0f);
	//Project = XMMatrixPerspectiveLH(win->getWidth(), win->getHeight(), 0.001f, 100.0f);
	//
	/*float zn = 0.001f,zf=100.0f;
	float yScale = 1.0f / tanf(XM_PIDIV2 / 2);
	float xScale = yScale / ((float)win->getWidth() / (float)win->getHeight());
	Project.r[0] = XMVectorSet(xScale, 0, 0, 0);
	Project.r[1] = XMVectorSet(0, yScale, 0, 0);
	Project.r[2] = XMVectorSet(0, 0, zf/(zf-zn), 1);
	Project.r[3] = XMVectorSet(0, 0, -zn*zf/(zf-zn), 0);
	
	*/
	Project = XMMatrixTranspose(Project);
	cb.SetProject(Project);
	cb.SetEniromentLight(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f));
	cb.SetLIghtDir(XMFLOAT3(0.0f, 0.0f, -1.0f));
	cb.SetLightColor(XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
	cylinder1->SetupConstantBuffer(cb.GetStr());
	cylinder2->SetupConstantBuffer(cb.GetStr());
	cylinder3->SetupConstantBuffer(cb.GetStr());
	cylinder4->SetupConstantBuffer(cb.GetStr());
	cylinder1->SetupBuffer();
	cylinder2->SetupBuffer();
	cylinder3->SetupBuffer();
	cylinder4->SetupBuffer();
	return S_OK;
}

void Direct3DApp::Render()
{
	float Color[] = { 1.0f,1.0f,1.0f,1.0f };
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, Color);


	float angle=ti->GetTickFromStart();
	XMMATRIX World;
	World = XMMatrixRotationX(angle/300.0f);
	World = XMMatrixTranspose(World);
	XMMATRIX World1, World2, World3, World4;
	XMMATRIX Down1 = XMMatrixTranslation(0.0f, 0.0f, -0.25f);
	XMMATRIX Down2 = XMMatrixTranslation(0.0f, 0.0f, -1.0f);
	XMMATRIX move1 = XMMatrixTranslation(2.0f, 2.0f, 0.0f);
	XMMATRIX move2 = XMMatrixTranslation(2.0f, -2.0f, 0.0f);
	XMMATRIX move3 = XMMatrixTranslation(-2.0f, -2.0f, 0.0f);
	XMMATRIX move4 = XMMatrixTranslation(-2.0f, 2.0f, 0.0f);

	World1 = XMMatrixTranspose(move1)*World*XMMatrixTranspose(Down1);	
	cb.SetWorld(World1);
	cylinder1->UpdateConstBuffer(cb.GetStr());


	World2 = XMMatrixTranspose(move2)*World*XMMatrixTranspose(Down2);
	cb.SetWorld(World2);
	cylinder2->UpdateConstBuffer(cb.GetStr());
	
	World3 = XMMatrixTranspose(move3)*World*XMMatrixTranspose(Down2);
	cb.SetWorld(World3);
	cylinder3->UpdateConstBuffer(cb.GetStr());
	
	World4 =XMMatrixTranspose(move4)*World*XMMatrixTranspose(Down2);
	cb.SetWorld(World4);
	cylinder4->UpdateConstBuffer(cb.GetStr());
	
	point1->PushToSwapChain();
	point2->PushToSwapChain();
	point3->PushToSwapChain();
	point4->PushToSwapChain();
	cylinder1->PushToSwapChain();
	cylinder2->PushToSwapChain();
	cylinder3->PushToSwapChain();
	cylinder4->PushToSwapChain();
	pSwapChain->Present(0, 0);
}

Direct3DApp::~Direct3DApp()
{
	
}
