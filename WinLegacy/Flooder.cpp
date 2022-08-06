#include "Flooder.h"

void Flooder::Initialize(HWND parent, HINSTANCE appInstance)
{
	FRCommon::Initialize(parent, appInstance);
	mBtnStart = CreateWindow(
		"BUTTON",                                               // Predefined class; Unicode assumed 
		"START",                                                // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		10,                                                     // x position 
		10,                                                     // y position 
		100,                                                    // Button width
		40,                                                     // Button height
		parent,                                                 // Parent window
		NULL,                                                   // No menu.
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		NULL);

	mBtnStop = CreateWindow(
		"BUTTON",                                               // Predefined class; Unicode assumed 
		"STOP",                                                 // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
		120,                                                    // x position 
		10,                                                     // y position 
		100,                                                    // Button width
		40,                                                     // Button height
		parent,                                                 // Parent window
		NULL,                                                   // No menu.
		(HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE),
		NULL);

	StartBackgroundThread();
}

void Flooder::HandleClick(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if ((LONG_PTR)lParam == (LONG_PTR)mBtnStart){
		
	}
	else if ((LONG_PTR)lParam == (LONG_PTR)mBtnStop) {
		
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