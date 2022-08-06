#include "Receiver.h"
#include <stdio.h>
#include "CommCtrl.h"

void Receiver::Initialize(HWND parent, HINSTANCE appInstance)
{
	FRCommon::Initialize(parent, appInstance);
	int widgetsLeft = 10;
	int widgetsTop = 10;
	int widgetsHeight = 22;
	int buddyWidth = 60;

	mNudMin = 0;
	mNudMax = 60;

	mHwndUpDownBuddy = CreateWindowEx(WS_EX_LEFT | WS_EX_CLIENTEDGE | WS_EX_CONTEXTHELP,
		WC_EDIT, NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_LEFT,// | ES_READONLY,
		widgetsLeft, widgetsTop, widgetsLeft + buddyWidth, widgetsHeight,
		mParent, NULL, (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE), NULL);

	mHwndUpDown = CreateWindow(UPDOWN_CLASS, NULL, WS_CHILD | WS_VISIBLE | UDS_ARROWKEYS,
		widgetsLeft + buddyWidth + 6, widgetsTop, buddyWidth, widgetsHeight,
		mParent, NULL, (HINSTANCE)GetWindowLongPtr(parent, GWLP_HINSTANCE), NULL);
	SendMessage(mHwndUpDown, UDM_SETRANGE, 0, MAKELPARAM(60, 0));
	mNudValue = 5;
	SetNudValue(mNudValue);
	StartBackgroundThread();
}

void Receiver::UpdateSharedData(SharedData* data) {
	data->mRcvGotData = true;
	data->mRcvPid = GetCurrentProcessId();
	data->mRcvHndl = (long)mParent;
}

void Receiver::SharedDataSynchronized() {
	FRCommon::SharedDataSynchronized();
}

int Receiver::GetCounterpartyPid() {
	return GetData().mSndPid;
}

void Receiver::OneSecondCycle() {

}

void Receiver::OnDummyMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	mDummyMessages++;
	char buff[64];
	sprintf_s(buff, "Dummy messages: %i", mDummyMessages);
	SetStatusText(buff);
}

void Receiver::OnFreezeMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Sleep(mNudValue * 1000);
}

void Receiver::OnNotifyMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	UINT nCode = ((LPNMHDR)lParam)->code;
	int iPosIndicated;
	LPNMUPDOWN lpnmud;

	switch (nCode)
	{
	case UDN_DELTAPOS:
		lpnmud = (LPNMUPDOWN)lParam;
		iPosIndicated = SendMessage(mHwndUpDown, UDM_GETPOS, (WPARAM)0, (LPARAM)0);
		int hi = HIWORD(iPosIndicated);
		int lo = LOWORD(iPosIndicated);
		int newPos = lo + lpnmud->iDelta;
		if (newPos < mNudMin || newPos > mNudMax) return;
		mNudValue = newPos;
		SetBuddyValue(newPos);
	}
}

void Receiver::SetNudValue(int value) {
	SendMessage(mHwndUpDown, UDM_SETPOS, (WPARAM)(0), (LPARAM)(value));
	SetBuddyValue(value);
}

void Receiver::SetBuddyValue(int value) {
	char status[16];
	sprintf_s(status, "%i", value);
	SendMessage(mHwndUpDownBuddy, WM_SETTEXT, 0, (LPARAM)status);
}