#include "Receiver.h"

void Receiver::Initialize(HWND parent, HINSTANCE appInstance)
{
	FRCommon::Initialize(parent, appInstance);

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