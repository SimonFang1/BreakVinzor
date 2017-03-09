#include "Driver.h"

#pragma comment(lib, "WinRing0.lib")

Driver::Driver() {
	if (!InitializeOls()) {
		// cannot initialize WinRing0
		errorcode = 13; // Permission denied
	} else {
		DWORD dllStatus = GetDllStatus();
		if (dllStatus) {
			// dll error found
			errorcode = 11;// Resource temporarily unavailable
		} else {
			// success
			errorcode = 0; // No error
		}
	}
}

Driver::~Driver() {
	DeinitializeOls();
}

bool Driver::good() {
	return errorcode == 0;
}

int Driver::getErrorCode() {
	return errorcode;
}

void Driver::KBCwait4IBE() {
	BYTE ch;
	// 自旋锁
	do {
		ch = ReadIoPortByte(P_KEY_CMD);//读取键盘的命令端口，得出ch
	} while ((ch & 0x2));//bit1是1的话，说明输入缓冲器已满
}

//按下
void Driver::MakeKeyDown(DWORD VirtualKey) {
	DWORD ScanCode = MapVirtualKey((BYTE)VirtualKey, 0);
	KBCwait4IBE();
	WriteIoPortByte(P_KEY_CMD, 0xd2);
	KBCwait4IBE();
	WriteIoPortByte(P_KEY_DAT, ScanCode);
}

//弹起
void Driver::MakeKeyUp(DWORD VirtualKey) {
	DWORD ScanCode = MapVirtualKey(VirtualKey, 0);
	KBCwait4IBE();
	WriteIoPortByte(P_KEY_CMD, 0xd2);
	KBCwait4IBE();
	WriteIoPortByte(P_KEY_CMD, ScanCode | 0x80);
}

void Driver::SendKey(const char ch) {
	DWORD sc, shift;
	unsigned char vkey;
	sc = OemKeyScan(ch);
	shift = sc >> 16;
	vkey = MapVirtualKey(sc & 0xffff, 1);
	if (shift)
		MakeKeyDown(VK_SHIFT);
	MakeKeyDown(vkey);
	MakeKeyUp(vkey);
	if (shift)
		MakeKeyUp(VK_SHIFT);
}

void Driver::SendString(const char *str) {
	DWORD sc, shift;
	unsigned char vkey;
	for (int i = 0; str[i] != '\0'; i++) {
		sc = OemKeyScan(str[i]);
		shift = sc >> 16;
		vkey = MapVirtualKey(sc & 0xffff, 1);
		if (shift)
			MakeKeyDown(VK_SHIFT);
		MakeKeyDown(vkey);
		MakeKeyUp(vkey);
		if (shift)
			MakeKeyUp(VK_SHIFT);
	}
}
