#include<Windows.h>
#include"Direct3DApp.h"
using namespace Tools;
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrev, char*CmdLine, int nCmd)
{
	Direct3DApp *app = new Direct3DApp(hinstance);
	app->run();
	return 0;
}