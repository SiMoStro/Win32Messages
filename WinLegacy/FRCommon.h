#pragma once
#include "Windows.h"
#include "SharedData.h"

class FRCommon
{
	private:
		int mSharedSize;
		LPDWORD mThreadId;
		HANDLE mThreadHndl;
		bool mKeepBckThreadAlive;
		SharedData mData;
		
		HANDLE hMapFile;
		LPCTSTR pSharedMemoryView;

		void OpenSharedMemory();
		void WriteLocalData();
		void ReadSharedData();

	protected: 
		bool mSharedDataOnce;
		HWND mParent;
		virtual void BackgroundProc() {};

	public: 
	
		FRCommon() {
			mSharedSize = 4096;
			mParent = 0;
			mThreadHndl = 0;
			mThreadId = 0;
			mKeepBckThreadAlive = false;
		}

		virtual void Initialize(HWND parent);

		void StartBackgroundThread();

		void StopBackgroundThread();

		virtual void BackgroundSingleCycle(){
			ReadSharedData();
			if (!mSharedDataOnce && mData.mRcvGotData && mData.mSndGotData) {
				mSharedDataOnce = true;
				UnmapViewOfFile(pSharedMemoryView);
                CloseHandle(hMapFile);
				SharedDataSynchronized();
			}
		}

		virtual void UpdateSharedData(SharedData* data) {};

		virtual void HandleClick(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){}

		bool GetBckThreadAlive() { return mKeepBckThreadAlive; }

		virtual void SharedDataSynchronized(){
			
		};
};

