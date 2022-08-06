#pragma once
#include "Windows.h"
#include "SharedData.h"

class FRCommon
{
	private:
		// Window and app instance
		HWND mHwndStatus;
		HINSTANCE mAppInstance;

		// background thread
		long mThreadCycle;
		LPDWORD mThreadId;
		HANDLE mThreadHndl = NULL;
		bool mKeepBckThreadAlive;
		
		// Shared memory
		int mSharedSize;
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

	public:
		static int mThreadSleep;
	
		FRCommon();

		virtual void Initialize(HWND parent, HINSTANCE appInstance);

		void StartBackgroundThread();

		void StopBackgroundThread();

		virtual void BackgroundSingleCycle();

		virtual void UpdateSharedData(SharedData* data) {};

		virtual void HandleClick(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){}

		virtual void HandleSize(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		bool GetBckThreadAlive() { return mKeepBckThreadAlive; }

		virtual void SharedDataSynchronized();

		void SetStatusText(const char* text);
};

