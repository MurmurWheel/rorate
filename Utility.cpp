#include "Utility.h"
namespace Utility
{
	HRESULT DirectBase::CompileShaderFromFile(wchar_t * fxFilename, char * EntryPoint, char * ShadeModel, ID3DBlob ** ppBlobOut)
	{
		HRESULT hr = S_OK;
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
		ID3DBlob* pErrorBlob;
		//编译着色器文件。

		//hr = D3DX11CompileFromFile(fxFilename, NULL, NULL, EntryPoint, ShadeModel,
		//dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
		hr = D3DCompileFromFile(fxFilename, NULL, NULL, EntryPoint, ShadeModel, NULL, NULL, ppBlobOut, &pErrorBlob);
		if (FAILED(hr)) {
			if (pErrorBlob != NULL)
				MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "Error", MB_OK);
			if (pErrorBlob)
				pErrorBlob->Release();
			return hr;
		}
		if (pErrorBlob)
			pErrorBlob->Release();
		return S_OK;
	}
	LRESULT DirectBase::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		return 0;
	}
	HRESULT DirectBase::setupSwapChain()
	{
		HRESULT hr = S_OK;
		UINT width = win->getWidth();
		UINT height = win->getHeight();

		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = win->getHwnd();
		sd.SampleDesc.Count = 2;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &pSwapChain, &pDevice, &featureLevel, &pImmediateContext);
			if (SUCCEEDED(hr))
				break;
		}
		if (FAILED(hr))
			return hr;

		// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
		if (FAILED(hr))
			return hr;

		hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL,&pRenderTargetView);
		pBackBuffer->Release();
		if (FAILED(hr))
			return hr;

		pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)width;
		vp.Height = (FLOAT)height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pImmediateContext->RSSetViewports(1, &vp);

		return S_OK;
	}
	void DirectBase::MessageLoop()
	{
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	DirectBase::DirectBase(HINSTANCE hinstance)
		:pSwapChain(nullptr),
		pRenderTargetView(nullptr),
		pDevice(nullptr),
		pImmediateContext(nullptr),
		win(new Tools::Window(hinstance)),
		hInstance(hinstance),
		featureLevel(D3D_FEATURE_LEVEL_11_0),
		driverType(D3D_DRIVER_TYPE_HARDWARE)
	{
		
	}
	void DirectBase::init()
	{
		win->setWndProc(WndProc);
		win->regisClass(L"WINDOW");
		
		win->setInitPos(200, 200);
		win->setWindowSize(800, 800);
		win->createWindow(L"WINDOW", Tools::WS_STYLE::ws_overlappedwindow);
		win->displayWindow();
	}
	void DirectBase::run()
	{
		this->MessageLoop();
	}
}

Utility::Figure2D::Points::Points(ID3D11Device * pd3dDevice, ID3D11DeviceContext * pContext,
	ID3D11PixelShader * pd3dPixelShader, ID3D11VertexShader * pd3dVertexShader,ID3D11InputLayout *pInput)
{
	ppDevice = pd3dDevice;
	ppImmediateContext = pContext;
	pPixelShader = pd3dPixelShader;
	pVertexShader = pd3dVertexShader;
	pInputLayout2D = pInput;
	pVertexBuffer = nullptr;
	vertexnum = 0;
	buffer = new SimplePC[8];
	size = 8;
	vertexnum = 0;
}

void Utility::Figure2D::Points::Setup()
{
	if (pVertexBuffer != nullptr)
		pVertexBuffer->Release();
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = sizeof(SimplePC)*vertexnum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = buffer;
	ppDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
}

void Utility::Figure2D::Points::PushToSwapChain()
{
	
	
	UINT stride = sizeof(SimplePC);
	UINT offset = 0;
	ppImmediateContext->IASetInputLayout(pInputLayout2D);
	ppImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	//ppImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	ppImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//
	ppImmediateContext->PSSetShader(pPixelShader, NULL, 0);
	ppImmediateContext->VSSetShader(pVertexShader, NULL, 0);
	ppImmediateContext->Draw(vertexnum, 0);
}

void Utility::Figure2D::Points::addapoint(XMFLOAT2 position, XMFLOAT4 Color)
{
	
	SimplePC simple;
	simple.Color = Color;
	simple.Pos.x = position.x;
	simple.Pos.y = position.y;
	simple.Pos.z = 0.0f;
	simple.Pos.w = 1.0f;
	if (vertexnum + 1 > size)
	{
		SimplePC* old = buffer;
		buffer = new SimplePC[2 * size];
		memcpy(buffer, old, size*sizeof(SimplePC));
		size = size * 2;
		delete[] old;
	}
	*(buffer + vertexnum) = simple;
	vertexnum = vertexnum + 1;
}

void Utility::Figure2D::Points::clear()
{
	vertexnum = 0;
	delete[]buffer;
	buffer = new SimplePC[8];
	size = 8;
}

void Utility::Figure2D::Points::Release()
{
	delete buffer;
	pVertexBuffer->Release();
}


Utility::Figure3D::Figure3D::Figure3D(ID3D11VertexShader * pVertex, ID3D11PixelShader * pPixel,
	ID3D11Device * Device, ID3D11DeviceContext * pImmediate,ID3D11InputLayout*pInput)
	:vertexnum(0),pConstBuffer(0),pVertexBuffer(0),pVertexShader(pVertex),
	pPixelShader(pPixel),pDevice(Device),pImmediateContext(pImmediate),pInputLayout3D(pInput)
{

}

Utility::Figure3D::Cylinder::Cylinder(ID3D11Device * pDev, ID3D11DeviceContext * pCon, ID3D11PixelShader * pPixel, 
	ID3D11VertexShader * pVertex,ID3D11InputLayout*pInput)
	:Figure3D(pVertex,pPixel,pDev,pCon,pInput),cb(nullptr)
{
}

void Utility::Figure3D::Cylinder::SetupBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = sizeof(SimplePN)*vertexnum;
	bd.Usage = D3D11_USAGE_DEFAULT;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = buffer;
	pDevice->CreateBuffer(&bd, &InitData, &pVertexBuffer);
	//Create Constant Buffer
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.ByteWidth = sizeof(ConstBuffer);
	if (cb == nullptr)
	{
		MessageBox(NULL, L"cb=0", L"ERROR", MB_OK);
		abort();
	}
	InitData.pSysMem = cb;
	pDevice->CreateBuffer(&bd, &InitData, &pConstBuffer);
}

UINT Utility::Figure3D::Cylinder::generate(UINT N,float r)
{
	vertexnum = N;
	buffer = new SimplePN[N];
	float _Xcos, _Ysin;
	for (UINT i = 0; i < N; i++) {
		float angle = XM_2PI / N * i;
		_Xcos = r*cosf(angle);
		_Ysin = r*sinf(angle);
		buffer[i].Pos.x = _Xcos;
		buffer[i].Pos.y = _Ysin;
		buffer[i].Pos.z = 0.0f;
		buffer[i].Pos.w = 1.0f;
		buffer[i].Dir = XMFLOAT4(0.0f, 0.0f, 1.0f,0.0f);
	}
	return 100;
}

UINT Utility::Figure3D::Cylinder::generate( UINT N, float rtop, float rbottom, float h)
{
	const UINT numVertex = 12 * N;
	vertexnum = numVertex;
	SimplePN *vertexTemp = new SimplePN[numVertex];
	float _Xcos1, _Ysin1, _Xcos2, _Ysin2;
	const float deltaAngle = XM_2PI / (float)N;
	float angle;
	XMFLOAT4 Direct;
	XMFLOAT4 a11, a12, a21, a22;
	const XMFLOAT4 top = XMFLOAT4(0.0f, 0.0f, h,1.0f);
	const XMFLOAT4 bottom = XMFLOAT4(0.0f, 0.0f, 0.0f,1.0f);
	for (UINT i = 0; i < N; i++) {
		UINT base = 12 * i;
		angle = i*deltaAngle;
		_Xcos1 = cosf(angle);				_Ysin1 = sinf(angle);
		_Xcos2 = cosf(angle + deltaAngle);	_Ysin2 = sinf(angle + deltaAngle);
		a11 = XMFLOAT4(rtop*_Xcos1, rtop*_Ysin1, h,1.0f);			a12 = XMFLOAT4(rtop*_Xcos2, rtop*_Ysin2, h,1.0f);
		a21 = XMFLOAT4(rbottom*_Xcos1, rbottom*_Ysin1, 0.0f,1.0f);	a22 = XMFLOAT4(rbottom*_Xcos2, rbottom*_Ysin2, 0.0f,1.0f);
		//侧面
		Direct = XMFLOAT4(_Xcos1 + _Xcos2, _Ysin1 + _Ysin2, 0.0f,0.0f);
		
		float sq = Direct.x*Direct.x + Direct.y*Direct.y;
		sq = sqrt(sq);
		Direct.x = Direct.x / sq; Direct.y = Direct.y / sq;
		vertexTemp[base].Pos = a11;			vertexTemp[base + 1].Pos = a12;			vertexTemp[base + 2].Pos = a22;
		vertexTemp[base].Dir = Direct;	vertexTemp[base + 1].Dir = Direct;	vertexTemp[base + 2].Dir = Direct;

		
		vertexTemp[base + 3].Pos = a22;			vertexTemp[base + 4].Pos = a21;			vertexTemp[base + 5].Pos = a11;
		vertexTemp[base + 3].Dir = Direct;	vertexTemp[base + 4].Dir = Direct;	vertexTemp[base + 5].Dir = Direct;
		//底面
		//顶
		Direct = XMFLOAT4(0.0f, 0.0f, 1.0f,0.0f);
		vertexTemp[base + 6].Pos = top;			vertexTemp[base + 7].Pos = a12;			vertexTemp[base + 8].Pos = a11;
		vertexTemp[base + 6].Dir = Direct;	vertexTemp[base + 7].Dir = Direct;	vertexTemp[base + 8].Dir = Direct;
		//底
		Direct = XMFLOAT4(0.0f, 0.0f, -1.0f,0.0f);
		vertexTemp[base + 9].Pos = bottom;		vertexTemp[base + 10].Pos = a21;			vertexTemp[base + 11].Pos = a22;
		vertexTemp[base + 9].Dir = Direct;	vertexTemp[base + 10].Dir = Direct;		vertexTemp[base + 11].Dir = Direct;
	}
	buffer = vertexTemp;
	return numVertex;
}

void Utility::Figure3D::Cylinder::SetupConstantBuffer(ConstBuffer * Cb)
{
	if (Cb == nullptr) {
		MessageBox(NULL, L"cb=null", L"ERROR", MB_OK);
		abort();
	}
	if (cb == nullptr)
		cb = new ConstBuffer(*Cb);
	else
		*cb = *Cb;
}

void Utility::Figure3D::Cylinder::PushToSwapChain()
{
	UINT offset = 0;
	UINT stride = sizeof(SimplePN);
	pImmediateContext->IASetInputLayout(pInputLayout3D);
	//pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	//设置数据源
	pImmediateContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pImmediateContext->VSSetConstantBuffers(0, 1, &pConstBuffer);
	pImmediateContext->PSSetConstantBuffers(0, 1, &pConstBuffer);
	//设置着色器
	pImmediateContext->PSSetShader(pPixelShader, NULL, 0);
	pImmediateContext->VSSetShader(pVertexShader, NULL, 0);
	//设置常量
	pImmediateContext->Draw(vertexnum, 0);
}

void Utility::Figure3D::Cylinder::UpdateConstBuffer(ConstBuffer * Cb)
{
	if (cb == nullptr)
		cb = new ConstBuffer(*Cb);
	else
		*cb = *Cb;
	pImmediateContext->UpdateSubresource(pConstBuffer, 0, NULL, cb, 0, 0);
}

void Utility::Figure3D::Cylinder::Release()
{
	delete cb;
	delete[] buffer;
	if(pConstBuffer!=nullptr)
		pConstBuffer->Release();
	if(pVertexBuffer!=nullptr)
		pVertexBuffer->Release();
}

