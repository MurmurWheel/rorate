#pragma once
#include<Windows.h>
namespace Tools
{
	enum WS_STYLE
	{
		//创建一个带边框的窗口
		ws_border=WS_BORDER,
		
		//创建一个有标题框的窗口（包括WS_BORDER风格）。
		ws_caption=WS_CAPTION,
		
		//创建一个子窗口。这个风格不能与WS_POPUP风格合用
		ws_child=WS_CHILD,
		
		//与WS_CHILD相同
		ws_childwindow=WS_CHILDWINDOW,
		
		//当在父窗口内绘图时，排除子窗口区域。在创建父窗口时使用这个风格
		ws_clipchildren=WS_CLIPCHILDREN,
		
		//排除子窗口之间的相对区域，也就是，当一个特定的窗口接收到WM_PAINT消息时，
		//WS_CLIPSIBLINGS 风格将所有层叠窗口排除在绘图之外，只重绘指定的子窗口。
		//如果未指定WS_CLIPSIBLINGS风格，并且子窗口是层叠的，
		//则在重绘子窗口的客户区时，就会重绘邻近的子窗口
		ws_clipsiblings=WS_CLIPSIBLINGS,
		
		//创建一个初始状态为禁止的子窗口。一个禁止状态的窗口不能接受来自用户的输入信息
		ws_disabled=WS_DISABLED,
		
		//创建一个带对话框边框风格的窗口。这种风格的窗口不能带标题条。
		ws_dlgframe=WS_DLGFRAME,
		
		//指定一组控制的第一个控制。这个控制组由第一个控制和随后定义的控制组成，
		//自第二个控制开始每个控制，具有WS_GROUP风格，每个组的第一个控制带有WS_TABSTOP风格，
		//从而使用户可以在组间移动。用户随后可以使用光标在组内的控制间改变键盘焦点。
		ws_group=WS_GROUP,
		
		//创建一个有水平滚动条的窗口
		ws_hscroll=WS_HSCROLL,
		
		//创建一个初始状态为最小化状态的窗口。与WS_MINIMIZE风格相同
		ws_iconic=WS_ICONIC,
		
		//创建一个初始状态为最大化状态的窗口
		ws_maximize=WS_MAXIMIZE,
		
		//创建一个具有最大化按钮的窗口。该风格不能与WS_EX_CONTEXTHELP风格同时出现，
		//同时必须指定WS_SYSMENU风格。
		ws_maximizebox=WS_MAXIMIZEBOX,
		
		//产生一个层叠的窗口。一个层叠的窗口有一个标题条和一个边框。与WS_TILED风格相同。
		ws_overlapped=WS_OVERLAPPED,
		//创建一个具有WS_OVERLAPPED，WS_CAPTION，WS_SYSMENU WS_THICKFRAME，
		//WS_MINIMIZEBOX，WS_MAXIMIZEBOX风格的层叠窗口，与WS_TILEDWINDOW风格相同
		ws_overlappedwindow=WS_OVERLAPPEDWINDOW,
												
		//创建一个弹出式窗口。该风格不能与WS_CHILD风格同时使用
		ws_popup=WS_POPUP,
		
		//创建一个具有WS_BORDER，WS_POPUP,WS_SYSMENU风格的窗口，
		//WS_CAPTION和WS_POPUPWINDOW必须同时设定才能使窗口某单可见
		ws_popupwindow=WS_POPUPWINDOW,
		
		//创建一个可调边框的窗口，与WS_THICKFRAME风格相同
		ws_sizebox=WS_SIZEBOX,
		
		//创建一个在标题条上带有窗口菜单的窗口，必须同时设定WS_CAPTION风格
		ws_sysmenu=WS_SYSMENU,
		
		//创建一个控制，这个控制在用户按下Tab键时可以获得键盘焦点。
		//按下Tab键后使键盘焦点转移到下一具有WS_TABSTOP风格的控制。
		ws_tabstop=WS_TABSTOP,
		
		//创建一个具有可调边框的窗口，与WS_SIZEBOX风格相同
		ws_thickframe=WS_THICKFRAME,
		
		//产生一个层叠的窗口。一个层叠的窗口有一个标题和一个边框
		ws_tiled=WS_TILED,
		
		//与WS_OVERLAPPEDWINDOW风格相同
		ws_tiledwindow=WS_TILEDWINDOW,
		
		//创建一个初始状态为可见的窗口
		ws_visible=WS_VISIBLE,
		
		//创建一个有垂直滚动条的窗口
		ws_vscroll=WS_VSCROLL
	};
	class Window
	{
		struct WindowSize
		{
			UINT height, width;
			wchar_t* WindowName;
		}WindowSize;
		HWND hwnd;
		HINSTANCE hinstance;
		UINT initX, initY;
		const HWND parentWindow;
		const HMENU  parentMenu;
		WNDPROC WndProc;
	public:
		Window(HINSTANCE hInstance);
		Window(HINSTANCE hInst, HWND parHwnd, HMENU parMenu);
		void setWindowName(wchar_t*newName);
		void setWindowSize(UINT w, UINT h) { WindowSize.height = h; WindowSize.width = w; }
		void setInitPos(UINT x, UINT y) { initX = x; initY = y; }
		void setWndProc(WNDPROC wndProc) { WndProc = wndProc; }
		void displayWindow(int nCmdShow = SW_SHOW);
		void createWindow(wchar_t*registedName);
		void createWindow(wchar_t*registedName,WS_STYLE dwStyle);
		void regisClass(wchar_t*classname);
		const HWND getHwnd();
		UINT getWidth();
		UINT getHeight();
		RECT getRect();
		void closeWindow();
	};
	class App
	{
	private:
		HINSTANCE hInstance;
		Window *win;
		
		static LRESULT _stdcall WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		void MessageLoop();
	public:
		App(HINSTANCE hinstance)
			:win(NULL), hInstance(hinstance) {}
		void run();
	};
	class Timer
	{
	private:
		DWORD64 Tick1, Tick2;
		DWORD64 StartPoint;
	public:
		Timer();
		DWORD64 GetTickDelta();
		void SetStartPoint();
		DWORD64 GetTickFromStart();
	};
}
