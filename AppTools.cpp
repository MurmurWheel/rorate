#include "AppTools.h"
namespace Tools
{
	Window::Window(HINSTANCE hInstance)
		:hwnd(0), WindowSize{ 500,500,L"DefaultWindow" },
		hinstance(hInstance), initX(0), initY(0),
		parentWindow(NULL), parentMenu(NULL),WndProc(NULL)
	{
	}
	Window::Window(HINSTANCE hInst, HWND parHwnd, HMENU parMenu)
		: hwnd(0), WindowSize{ 500,500,L"DefaultWindow" }, 
		hinstance(hInst), initX(0), initY(0), 
		parentWindow(parHwnd), parentMenu(parMenu),
		WndProc(NULL)
	{}
	void Window::setWindowName(wchar_t*newName) 
	{ 
		WindowSize.WindowName = newName; 
	}
	void Window::displayWindow(int nCmdShow)
	{
		ShowWindow(hwnd, nCmdShow);
	}
	void Window::createWindow(wchar_t*registedClass)
	{
		hwnd = CreateWindow(registedClass,
			WindowSize.WindowName,
			WS_OVERLAPPEDWINDOW,
			initX, initY,
			WindowSize.width, WindowSize.height,
			parentWindow, parentMenu,
			hinstance, 0);
	}
	void Window::createWindow(wchar_t*classname,WS_STYLE dwstyle)
	{
		hwnd = CreateWindow(classname,
			WindowSize.WindowName,
			dwstyle,
			initX, initY,
			WindowSize.width, WindowSize.height,
			parentWindow, parentMenu,
			hinstance, 0);
		if (hwnd == NULL)
			MessageBox(NULL, L"Create Window Failed!", L"ERROR", MB_OK);
	}
	void Window::regisClass(wchar_t * classname)
	{
		WNDCLASS wc;
		ZeroMemory(&wc, sizeof(WNDCLASS));
		if (WndProc == NULL) {
			MessageBox(NULL, L"WndProc==0", L"ERROR", MB_OK);
			abort();
			return;
		}
		wc.style = 0;
		wc.lpfnWndProc = WndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hinstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(GetStockObject(WHITE_BRUSH));
		wc.lpszMenuName = NULL;
		wc.lpszClassName = classname;
		if (!RegisterClass(&wc))
		{
			MessageBox(NULL, L"Register Failed!", L"ERROR", MB_OK);
			abort();
			return;
		}
	}
	const HWND Window::getHwnd()
	{
		if (hwnd == 0)
		{
			MessageBox(NULL, L"HWND=0", L"ERROR", MB_OK);
		}
		return hwnd;
	}
	UINT Window::getHeight()
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		return rc.bottom - rc.top;
	}
	RECT Window::getRect()
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		return rc;
	}
	UINT Window::getWidth()
	{
		RECT rc;
		GetClientRect(hwnd, &rc);
		return rc.right - rc.left;
	}
	void Window::closeWindow()
	{
		CloseWindow(hwnd);
	}
	LRESULT _stdcall App::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	void App::MessageLoop()
	{
		MSG msg;
		while (GetMessage(&msg,NULL,0,0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	void App::run()
	{
		win = new Window(hInstance);
		win->setWndProc(WndProc);
		win->regisClass(L"WINDOW");
		win->createWindow(L"WINDOW");
		win->setWindowName(L"DEFAULT WINDOW");
		win->displayWindow();
		MessageLoop();
	}
	Timer::Timer()
		:Tick1(0),Tick2(0)
	{
	}
	DWORD64 Timer::GetTickDelta()
	{
		Tick2 = GetTickCount64();
		DWORD64 res = Tick2 - Tick1;
		Tick1 = Tick2;
		return res;
	}
	void Timer::SetStartPoint()
	{
		StartPoint = GetTickCount64();
	}
	DWORD64 Timer::GetTickFromStart()
	{
		return GetTickCount64() - StartPoint;
	}
}
