#include "Windows.h"
#include <stdio.h>
#include "FRCommon.h"
#include "Receiver.h"
#include "Flooder.h"

WNDCLASS wc;
FRCommon* mUserInterface;

#define WM_USERSTART (WM_USER + 10)
#define WM_USEREND (WM_USER + 20)
#define WM_USERDUMMY (WM_USER + 30)

LRESULT CALLBACK MyWindowCMessageLoop(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char buffer[256] = { 0 };

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		// All painting occurs here, between BeginPaint and EndPaint.
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 2));
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
		if (mUserInterface != NULL) {
			mUserInterface->HandleClick(hwnd, uMsg, wParam, lParam);
		}
		break;
	case 512:
	case 132:
	case 32:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);

	default:
		//sprintf_s(buffer, "Message: %d; dest: %p; (%p, %p)\n", uMsg, hwnd, wParam, lParam);
		//OutputDebugString(buffer);
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
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
	//wc.lpszMenuName = "MainMenu";

	if (!RegisterClass(&wc)) return false;
	return true;
}

void SetupUserInterface(HWND parent)
{
	if (mUserInterface != NULL) {
		mUserInterface->Initialize(parent);
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

	HWND hwndMain = CreateWindow(wc.lpszClassName, "WinMain", WS_OVERLAPPEDWINDOW, 100, 100, 400, 300, NULL, NULL, hInstance, (LPVOID)NULL);
	if (!hwndMain) return FALSE;

	ShowWindow(hwndMain, SW_SHOW);
	UpdateWindow(hwndMain);

	SetupUserInterface(hwndMain);

	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}