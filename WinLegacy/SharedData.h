#pragma once

struct SharedData {
	// Receiver PID
	long mRcvPid;
	// Receiver window handle
	long mRcvHndl;
	// Receiver got shared data
	bool mRcvGotData;
	// Sender PID
	long mSndPid;
	// Sender window handle
	long mSndHndl;	
	// Sender got shared data
	bool mSndGotData;

	void Reset() {
		mRcvGotData = mSndGotData = false;
		mRcvPid = mSndPid = 0;
		mRcvHndl = mSndHndl = NULL;
	}
};