#ifndef __WINRING0_DRIVER_H__
#define __WINRING0_DRIVER_H__

#include "windows.h"
#include "OlsApi.h"

enum Port {
	P_KEY_CMD = 0x64,
	P_KEY_DAT = 0x60
};

class Driver {
public:
	Driver();
	~Driver();
	void MakeKeyDown(DWORD VirtualKey);
	void MakeKeyUp(DWORD VirtualKey);
	void SendKey(const char ch);
	void SendString(const char *str);
	void KBCwait4IBE();	//µÈ´ý»º³åÇøÎª¿Õ
	bool good();
	int getErrorCode();
private:
	int errorcode;
};

#endif
