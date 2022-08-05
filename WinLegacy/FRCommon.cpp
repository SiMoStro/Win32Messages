#include "FRCommon.h"
#include <stdio.h>
#include <conio.h>

DWORD WINAPI WorkingThread(_In_ LPVOID lpParameter)
{
	FRCommon* ci = (FRCommon*)lpParameter;
	while (ci->GetBckThreadAlive()) {
		ci->BackgroundSingleCycle();
		Sleep(100);
	}

	return 0;
}

void FRCommon::Initialize(HWND parent) {
	mParent = parent;

	OpenSharedMemory();
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

void FRCommon::ReadSharedData() {
	if (mSharedDataOnce || pSharedMemoryView == NULL) {
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
	int sz = sizeof(mData);
	UpdateSharedData(&mData);
	CopyMemory((PVOID)pSharedMemoryView, &mData, sizeof(mData));
}
