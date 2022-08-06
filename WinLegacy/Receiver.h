#pragma once
#include "FRCommon.h"
#include "Windows.h"

class Receiver : public FRCommon
{
	void Initialize(HWND parent, HINSTANCE appInstance) override;

	void UpdateSharedData(SharedData* data) override;

	void SharedDataSynchronized() override;

	int GetCounterpartyPid() override;
};

