#include "FRCommon.h"
#include "CommCtrl.h"
#include <stdio.h>
#include <conio.h>

int FRCommon::mThreadSleep = 100;

DWORD WINAPI WorkingThread(_In_ LPVOID lpParameter)
{
	FRCommon* ci = (FRCommon*)lpParameter;
	while (ci->GetBckThreadAlive()) {
		ci->BackgroundSingleCycle();
		Sleep(FRCommon::mThreadSleep);
	}

	return 0;
}

FRCommon::FRCommon() {
	mSharedSize = 4096;
	mParent = 0;
	mThreadHndl = 0;
	mThreadId = 0;
	mKeepBckThreadAlive = false;
}

void FRCommon::Initialize(HWND parent, HINSTANCE hInstance) {
	mParent = parent;
	mAppInstance = hInstance;

	OpenSharedMemory();
	CreateStatusBar();
	SetStatusText("Waiting counterparty...");
}

void FRCommon::StartBackgroundThread()
{
	mKeepBckThreadAlive = true;
	mThreadHndl = CreateThread(NULL, 0, WorkingThread, this, 0, mThreadId);
}

void FRCommon::StopBackgroundThread() {
	mKeepBckThreadAlive = false;
}

void FRCommon::OpenSharedMemory() {
	TCHAR szName[] = TEXT("FlooderReceiverSample");

	hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, mSharedSize, szName);
	if (hMapFile == NULL) {
		DWORD err = GetLastError();
		return;
	}

	pSharedMemoryView = (LPTSTR)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, mSharedSize);
	if (pSharedMemoryView == NULL) {
		DWORD err = GetLastError();
		CloseHandle(hMapFile);
		return;
	}

	WriteLocalData();
}

void FRCommon::BackgroundSingleCycle() {
	mThreadCycle++;
	bool isSecond = (mThreadCycle * FRCommon::mThreadSleep) % 1000 == 0;
	if (isSecond && mSynchronized)
	{ // every second we check if counterparty is still alive
		int pid = GetCounterpartyPid();
		if (pid != 0) {
			DWORD exitCode;
			HANDLE process = NULL;
			process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
			BOOL gotExitCode = GetExitCodeProcess(process, &exitCode);
			CloseHandle(process);
			if (exitCode != STILL_ACTIVE) {
				mSynchronized = false;
				OpenSharedMemory();
				SetStatusText("Lost connection - waiting counterparty...");
			}

		}
	}

	// check if counterparty synchronized data
	if (!mSynchronized) {
		ReadSharedData();
		if (mData.mRcvGotData && mData.mSndGotData) {
			mSynchronized = true;
			UnmapViewOfFile(pSharedMemoryView);
			CloseHandle(hMapFile);
			SharedDataSynchronized();
		}		
	}
}

void FRCommon::ReadSharedData() {
	if (mSynchronized || pSharedMemoryView == NULL) {
		return;
	}
	int sz = sizeof(mData);
	CopyMemory(&mData, (PVOID)pSharedMemoryView, sizeof(mData));
}

void FRCommon::WriteLocalData() {
	if (pSharedMemoryView == NULL) {
		return;
	}

	ReadSharedData();
	UpdateSharedData(&mData);
	int sz = sizeof(mData);
	CopyMemory((PVOID)pSharedMemoryView, &mData, sizeof(mData));
}

void FRCommon::CreateStatusBar() {
	if (mParent == NULL) {
		return;
	}

	mHwndStatus = CreateWindowEx(0, STATUSCLASSNAME, (PCTSTR)NULL, SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, mParent, NULL, mAppInstance, NULL);
	SendMessage(mHwndStatus, WM_SIZE, 0, 0);
}

void FRCommon::HandleSize(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (mHwndStatus != NULL) {
		SendMessage(mHwndStatus, WM_SIZE, 0, 0);
	}
}

void FRCommon::SetStatusText(const char* text) {
	if (mHwndStatus != NULL) {
		SendMessage(mHwndStatus, SB_SETTEXT, 0, (LPARAM)text);
	}
}

void FRCommon::SharedDataSynchronized() {
	char status[512];
	sprintf_s(status, "Cross-app synch complete; Snd: %i; Rcv: %i", mData.mSndPid, mData.mRcvPid);
	SetStatusText(status);
};