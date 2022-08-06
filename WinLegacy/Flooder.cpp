#include "Flooder.h"
#include "CommonDef.h"

void Flooder::Initialize(HWND parent, HINSTANCE appInstance)
{
	FRCommon::Initialize(parent, appInstance);
	mBtnStart = CreateWindow("BUTTON", "Start", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 10, 100, 40, parent, NULL, (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE), NULL);

	mBtnStop = CreateWindow("BUTTON", "Stop", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		120, 10, 100, 40, parent, NULL, (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE), NULL);

	mBtnFreeze = CreateWindow("BUTTON", "Freeze", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		10, 60, 210, 40, parent, NULL, (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE), NULL);

	StartBackgroundThread();
}

void Flooder::HandleClick(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if ((LONG_PTR)lParam == (LONG_PTR)mBtnStart){
		mShouldSendMessages = true;
	}
	else if ((LONG_PTR)lParam == (LONG_PTR)mBtnStop) {
		mShouldSendMessages = false;
	}
	else if ((LONG_PTR)lParam == (LONG_PTR)mBtnFreeze) {
		if (mSynchronized && mShouldSendMessages) {
			PostMessage((HWND)GetData().mRcvHndl, WM_USERFREEZE, 0, 0);
		}
	}
}

void Flooder::BackgroundSingleCycle() {
	FRCommon::BackgroundSingleCycle();
}

void Flooder::UpdateSharedData(SharedData* data) {
	data->mSndGotData = true;
	data->mSndPid = GetCurrentProcessId();
	data->mSndHndl = (long)mParent;
}

void Flooder::SharedDataSynchronized() {
	FRCommon::SharedDataSynchronized();
}

int Flooder::GetCounterpartyPid() {
	return GetData().mRcvPid;
}

void Flooder::OneSecondCycle() {
	if (mSynchronized && mShouldSendMessages) {
		PostMessage((HWND)GetData().mRcvHndl, WM_USERDUMMY, 0, 0);
	}
}