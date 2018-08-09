#include "MainApp.h"



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR args, int argv) {

	MSG msg;
	HWND hWnd;
	WNDCLASS wnc;
	
	wnc.cbClsExtra = NULL;
	wnc.cbWndExtra = NULL;
	wnc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wnc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wnc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wnc.hInstance = hInstance;
	wnc.lpfnWndProc = &mainProc;
	wnc.lpszClassName = "alvWINDOW";
	wnc.lpszMenuName = NULL;
	wnc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;	//CS_DBLCLKS 是为了让窗口能够响应双击事件

	//创建窗口类,以及设置窗口类的信息

	if (!RegisterClass(&wnc)) {	//注册窗口类,失败则退出
		MessageBox(0, "ERROR IN REGISTER CLASS", "ERROR", MB_ICONERROR);
		return -1;
	}

	hWnd = CreateWindow(wnc.lpszClassName, "Window", CW_USEDEFAULT,		
			CW_USEDEFAULT, CW_USEDEFAULT, 300, 300, NULL, NULL, hInstance, NULL);
	//创建窗口

	if (hWnd == NULL) {			//失败则退出
		MessageBox(0, "ERROR IN CREATE WINDOW", "ERROR", MB_ICONERROR);
		return -1;
	}
	
	LONG wndExStyle = GetWindowLong(hWnd, GWL_EXSTYLE);	//给窗口添加 WS_EX_TOOLWINDOW 样式,使窗口不在任务栏上显示
	wndExStyle |= WS_EX_TOOLWINDOW;
	SetWindowLong(hWnd, GWL_EXSTYLE, wndExStyle);

	

	ShowWindow(hWnd, SW_SHOW);			//显示窗口
	UpdateWindow(hWnd);					//更新窗口

	//SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

	while (GetMessage(&msg, NULL, NULL, NULL)) {	//进入消息循环
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK mainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
		
	case WM_CREATE:			//窗口创建消息响应
		OnCreate(hWnd, msg, wParam, lParam);
		RegisterHotKey(hWnd, ID_KEY_SHOWxHIDE, MOD_ALT, 'X');
		break;

	case WM_LBUTTONDBLCLK:
		ShowWindow(hWnd, SW_HIDE);
		break;
	case WM_LBUTTONDOWN:	//鼠标左键按下消息响应
		SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);				//给标题栏发送鼠标按下的消息,欺骗系统
		break;
	case WM_MOVE:
		StayWindowSide(hWnd);
		break;
	case WM_RBUTTONUP:
		OnRButtonUp(hWnd, msg, wParam, lParam);
		
		break;
	case WM_COMMAND:
		OnBtnClick(hWnd, msg, wParam, lParam);
		break;
	case NM_MYTrayMSG:
		OnTrayMsg(hWnd, msg, wParam, lParam);
		break;
	case WM_HOTKEY:
		if (wParam == ID_KEY_SHOWxHIDE) {
			if (IsWindowVisible(hWnd))
				ShowWindow(hWnd, SW_HIDE);
			else
				ShowWindow(hWnd, SW_SHOW);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}



