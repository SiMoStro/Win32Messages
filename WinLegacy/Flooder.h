#pragma once
#include "FRCommon.h"
#include "Windows.h"

class Flooder : public FRCommon
{
private:
	HWND mBtnStart;
	HWND mBtnStop;
public:
	void Initialize(HWND parent, HINSTANCE appInstance) override;
	
	void HandleClick(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	
	void BackgroundSingleCycle() override;
	
	void UpdateSharedData(SharedData* data) override;
	
	void SharedDataSynchronized() override;
	
	int GetCounterpartyPid() override;
};

