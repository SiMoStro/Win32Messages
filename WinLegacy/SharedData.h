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
};