#include "MainApp.h"

class BtnData {
public:
	HWND hBtn;

	String name;
	String cmd;
	String parm;
	String path;
private:

};

BtnData btns[20];
NOTIFYICONDATA TrayIcon;
LONG MenuCheckState = MF_CHECKED;

void OnCreate(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//查找并打开配置文件
	std::string line;
	std::ifstream confile("CmdList.conf");

	int ibtn = 0;
	int ix = 0, iy = 1;

	if (!confile) {
		MessageBox(hWnd, "ERROR IN OPEN CONFIG FILE", "ERROR", MB_ICONERROR);
		PostQuitMessage(0);
	}

	while (std::getline(confile, line)) {

		if (ix >= 2) {
			ix = 0;
			iy++;
		}

		if (line[0] == '#') continue;		//判断是否是注释
		
		int iDivName, iDivPath, iDivParm;

		iDivName = line.find("=");			//查找按钮名称和命令的分隔符
		if (iDivName < 0) continue;			//如果一行中没有分隔符"=",就跳过这一行

		btns[ibtn].cmd = line.substr(iDivName + 1, line.length() - 1);
		btns[ibtn].name = line.substr(0, iDivName);

		iDivParm = btns[ibtn].cmd.find("|");
		if (iDivParm >= 0) {
			btns[ibtn].parm = btns[ibtn].cmd.substr(iDivParm + 1, btns[ibtn].cmd.length() - 1);
			btns[ibtn].cmd = btns[ibtn].cmd.substr(0, iDivParm);
		}

		iDivPath = btns[ibtn].parm.find("|");
		if (iDivPath >= 0) {
			btns[ibtn].path = btns[ibtn].parm.substr(iDivPath + 1, btns[ibtn].parm.length() - 1);
			btns[ibtn].parm = btns[ibtn].parm.substr(0, iDivPath);
		}

		LONG ICONSHOW = NULL;

		if (btns[ibtn].name == "") ICONSHOW = BS_ICON;

		btns[ibtn].hBtn =  CreateWindow("button", btns[ibtn].name.c_str(), BS_PUSHBUTTON  | BS_MULTILINE | WS_VISIBLE | WS_CHILD | ICONSHOW,
								BTN_WID * ix++, BTN_WID*(iy), BTN_WID, BTN_WID,
								hWnd, (HMENU)(ID_BTN00 + ibtn), ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		

		SHFILEINFO shfi;
		SHGetFileInfo(btns[ibtn].cmd.c_str(), FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM, 
				&shfi, sizeof(SHFILEINFO), SHGFI_ICON);

		SendMessage(btns[ibtn].hBtn, BM_SETIMAGE, IMAGE_ICON, (LPARAM)shfi.hIcon);



		if (ibtn >= 19) break;				//最多二十个按钮
		ibtn++;
	}

	confile.close();

	std::ifstream posfile("Position.conf");
	int x = 0, y = 0;

	if (posfile) {
		while (std::getline(posfile, line))continue;
		int t = line.find(",");

		if (t>=0) {
			x = atoi(line.substr(0, t).c_str());
			y = atoi(line.substr(t + 1, line.length() - 1).c_str());
			
		}


	}else {
		int cx = GetSystemMetrics(SM_CXFULLSCREEN);
		int cy = GetSystemMetrics(SM_CYFULLSCREEN);
		x = (cx - BTN_WID);
		y = (cy - BTN_WID*ibtn) / 2;
	}

	posfile.close();
	if (iy > 1)
		SetWindowPos(hWnd, HWND_TOPMOST, x, y, BTN_WID * 2, BTN_WID * (iy + 1), NULL);
		//MoveWindow(hWnd, x, y, BTN_WID *2, BTN_WID * (iy+1), true);
	else
		SetWindowPos(hWnd, HWND_TOPMOST, x, y, BTN_WID *ix, BTN_WID * (iy + 1), NULL);
		//MoveWindow(hWnd, x, y, BTN_WID *ix, BTN_WID * (iy + 1), true);

	TrayIcon.cbSize = sizeof(NOTIFYICONDATA);
	TrayIcon.hIcon = LoadIcon(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDI_ICON1));
	TrayIcon.hWnd = hWnd;
	lstrcpy(TrayIcon.szTip, "");
	TrayIcon.uCallbackMessage = NM_MYTrayMSG;
	TrayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &TrayIcon);   //添加系统托盘
}

void OnBtnClick(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	if (LOWORD(wParam) >= ID_BTN00 && LOWORD(wParam) <= ID_BTN15) {
		if (HIWORD(wParam) == BN_CLICKED) {

			int i = LOWORD(wParam) - ID_BTN00;
			//MessageBox(hWnd, btns[i].cmd.c_str(), "", 0);
			if(btns[i].cmd != "")
				ShellExecute(0, "open", btns[i].cmd.c_str(), btns[i].parm.c_str(), btns[i].path.c_str(), SW_SHOW);
		}
		
	}
	else {
		switch (LOWORD(wParam))
		{
		case ID_POPMENU_EXIT:
			CloseApp(hWnd);
			break;
		case ID_POPMENU_HIDE:
			if (IsWindowVisible(hWnd))
				ShowWindow(hWnd, SW_HIDE);
			else
				ShowWindow(hWnd, SW_SHOW);
			break;
		case ID_POPMENU_RESTART:
			char filename[MAX_PATH];
			GetModuleFileName(NULL, filename, MAX_PATH);
			ShellExecute(0, "open", filename, "", "", SW_SHOW);
			CloseApp(hWnd);
			break;
		case ID_POPMENU_TOPMOST:
			if (MenuCheckState == MF_CHECKED) {
				MenuCheckState = MF_UNCHECKED;
				SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			else {
				MenuCheckState = MF_CHECKED;
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}

			break;
		}
	}

}

void OnTrayMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
	case WM_LBUTTONDBLCLK:
		if (IsWindowVisible(hWnd))
			ShowWindow(hWnd, SW_HIDE);
		else
			ShowWindow(hWnd, SW_SHOW);
		break;
	case WM_RBUTTONUP:
		SendMessage(hWnd, WM_RBUTTONUP, wParam, lParam);
		break;
	}
}

void OnRButtonUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HMENU M;
	M = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU1));
	HMENU popm = GetSubMenu(M, 0);

	if (IsWindowVisible(hWnd))
		ModifyMenu(popm, ID_POPMENU_HIDE, MF_BYCOMMAND, ID_POPMENU_HIDE, "&Hide");
	else
		ModifyMenu(popm, ID_POPMENU_HIDE, MF_BYCOMMAND, ID_POPMENU_HIDE, "&Show");

	CheckMenuItem(popm, ID_POPMENU_TOPMOST, MenuCheckState);

	POINT p;
	GetCursorPos(&p);

	TrackPopupMenu(popm, NULL, p.x, p.y, NULL, hWnd, NULL);
}

void DelTrayIcon()
{
	Shell_NotifyIcon(NIM_DELETE, &TrayIcon);
}

void CloseApp(HWND hWnd)
{
	std::ofstream posfile("Position.conf", std::ios::app);

	if (posfile) {
		String posline;
		RECT rect;
		GetWindowRect(hWnd, &rect);

		posline = (std::to_string(rect.left) + "," + std::to_string(rect.top));
		posfile <<std::endl << posline;
	}

	DelTrayIcon();
	PostQuitMessage(0);
}

void StayWindowSide(HWND hWnd)
{
	RECT rect;
	BOOL touchSide = false;

	LONG scx = GetSystemMetrics(SM_CXFULLSCREEN);
	LONG scy = GetSystemMetrics(SM_CYFULLSCREEN);

	if (GetWindowRect(hWnd, &rect)) {
		LONG x = rect.left;
		LONG y = rect.top;
		if (rect.left <= 0) {
			x = 0; 
			touchSide = true;
		}

		if (rect.top <= 0) {
			y = 0; 
			touchSide = true;
		}

		if (rect.bottom > scy) {
			y = scy - rect.bottom + rect.top;
			touchSide = true;
		}

		if (rect.right > scx) {
			x = scx - rect.right + rect.left;
			touchSide = true;
		}

		if (touchSide) SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}




}





