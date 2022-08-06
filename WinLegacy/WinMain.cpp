#include "Windows.h"
#include <stdio.h>
#include "FRCommon.h"
#include "Receiver.h"
#include "Flooder.h"
#include "CommonDef.h"

WNDCLASS wc;
FRCommon* mUserInterface;

LRESULT CALLBACK MyWindowCMessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char buffer[256] = { 0 };
	LRESULT res = 0;
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = 300;
		lpMMI->ptMinTrackSize.y = 200;
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
		if (mUserInterface != NULL) {
			mUserInterface->HandleClick(hwnd, uMsg, wParam, lParam);
		}
		break;
	case WM_SIZE:
		if (mUserInterface != NULL) {
			mUserInterface->OnSize(hwnd, uMsg, wParam, lParam);
		}
		break;
	case WM_USERDUMMY:
		if (mUserInterface != NULL) {
			mUserInterface->OnDummyMessage(hwnd, uMsg, wParam, lParam);
		}
		break;
	case WM_USERFREEZE:
		if (mUserInterface != NULL) {
			mUserInterface->OnFreezeMessage(hwnd, uMsg, wParam, lParam);
		}
		break;
	case WM_NOTIFY:
		if (mUserInterface != NULL) {
			OutputDebugString(buffer);
			mUserInterface->OnNotifyMessage(hwnd, uMsg, wParam, lParam);
		}
		break;
	default:
		res = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return res;
}

bool RegisterWindowClass(HINSTANCE hInstance)
{
	wc.lpfnWndProc = (WNDPROC)MyWindowCMessageLoop;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = "mySpecialWin";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor((HINSTANCE)NULL, IDC_ARROW);

	if (!RegisterClass(&wc)) return false;
	return true;
}

void SetupUserInterface(HWND parent, HINSTANCE hInstance)
{
	if (mUserInterface != NULL) {
		mUserInterface->Initialize(parent, hInstance);
	}
}

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	MSG msg;

	if (wcsstr(lpCmdLine, L"-flooder"))
	{
		mUserInterface = new Flooder();
	}
	else if (wcsstr(lpCmdLine, L"-receiver"))
	{
		mUserInterface = new Receiver();
	}

	if (hPrevInstance) return 1;
	if (!RegisterWindowClass(hInstance)) return FALSE;

	HWND hwndMain = CreateWindow(wc.lpszClassName, "WinMain", WS_OVERLAPPEDWINDOW, 100, 100, 320, 240, NULL, NULL, hInstance, (LPVOID)NULL);
	if (!hwndMain) return FALSE;

	ShowWindow(hwndMain, SW_SHOW);
	UpdateWindow(hwndMain);

	SetupUserInterface(hwndMain, hInstance);

	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}