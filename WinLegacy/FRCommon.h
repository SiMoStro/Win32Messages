#pragma once
#include "Windows.h"
#include "SharedData.h"

class FRCommon
{
	private:
		// Window and app instance
		HWND mHwndStatus = NULL;
		HINSTANCE mAppInstance = NULL;

		// background thread
		long mThreadCycle = 0;
		LPDWORD mThreadId = 0;
		HANDLE mThreadHndl = NULL;
		bool mKeepBckThreadAlive = false;
		
		// Shared memory
		int mSharedSize = 0;
		SharedData mData;
		HANDLE hMapFile = NULL;
		LPCTSTR pSharedMemoryView;

		void OpenSharedMemory();

		void WriteLocalData();

		void ReadSharedData();

	protected: 
		bool mSynchronized;
		HWND mParent = NULL;

		virtual void BackgroundProc() {};

		virtual int GetCounterpartyPid() { return 0; }

		void CreateStatusBar();

		SharedData GetData() { return mData; }

		virtual void OneSecondCycle(){};

	public:
		static int mThreadSleep;
	
		FRCommon();

		virtual void Initialize(HWND parent, HINSTANCE appInstance);

		void StartBackgroundThread();

		void StopBackgroundThread();

		virtual void BackgroundSingleCycle();

		virtual void UpdateSharedData(SharedData* data) {};

		virtual void HandleClick(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){}

		virtual void OnSize(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool GetBckThreadAlive() { return mKeepBckThreadAlive; }

		virtual void SharedDataSynchronized();

		void SetStatusText(const char* text);

		virtual void OnDummyMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual void OnFreezeMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual void OnNotifyMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

