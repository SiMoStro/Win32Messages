#pragma once
#include "FRCommon.h"
#include "Windows.h"

class Receiver : public FRCommon
{
private:
	long mDummyMessages = 0;
	HWND mHwndUpDown = NULL;
	HWND mHwndUpDownBuddy = NULL;
	int mNudMin, mNudMax;
	int mNudValue;

	void SetNudValue(int value);

	void SetBuddyValue(int value);

public:

	void Initialize(HWND parent, HINSTANCE appInstance) override;

	void UpdateSharedData(SharedData* data) override;

	void SharedDataSynchronized() override;

	int GetCounterpartyPid() override;

	void OneSecondCycle() override;

	void OnDummyMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void OnFreezeMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	void OnNotifyMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
};

