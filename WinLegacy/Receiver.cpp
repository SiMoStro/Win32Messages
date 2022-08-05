#include "Receiver.h"

void Receiver::Initialize(HWND parent)
{
	FRCommon::Initialize(parent);

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