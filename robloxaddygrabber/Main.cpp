#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

#define convert(a)(a + 0x400000 - (DWORD)GetModuleHandleA(0))

struct AOBPart {
	string name;
	const char* first;
	const char* second;
};

namespace AOB {
	bool Check(const BYTE* pd, const BYTE* aob, const char* mask)
	{
		for (; *mask; ++mask, ++pd, ++aob)
			if (*mask != '?' && *pd != *aob)
				return false;

		return (*mask) == NULL;
	}

	DWORD FindPattern(const char* aob, const char* mask)
	{
		for (DWORD ind = (DWORD)GetModuleHandle(0); ind <= 0xFFFFFFF; ++ind) {
			if (Check((BYTE*)ind, (BYTE*)aob, mask))
				return ind;
		}
		return 0x00000000;
	}
}

void main() {
	AOBPart aobs[] = { AOBPart{"openstate", "\x55\x8B\xEC\x56\x57\x6A\x05\xFF\x75\x08\x8B\xF9\x33\xF6\xE8\x00\x00\x00\x00", "xxxxxxxxxxxxxxx????"}, AOBPart{"retcheck", "\x55\x8B\xEC\x64\xA1\x00\x00\x00\x00\x6A\xFF\x68\x00\x00\x00\x00\x50\x64\x89\x25\x00\x00\x00\x00\x83\xEC\x0C\x57\x6A\x00", "xxxxx????xxx????xxxx????xxxxxx"} };

	for (AOBPart aob : aobs) {
		cout << aob.name << " - 0x" << std::hex << convert(AOB::FindPattern(aob.first, aob.second)) << endl;
	}
}

void CreateConsole() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	DWORD OldPerm, OldPermMsg, Base;


	VirtualProtect(&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &OldPerm);
	VirtualProtect(&MessageBoxA, 5, PAGE_EXECUTE_READWRITE, &OldPerm);
	*(BYTE*)(&FreeConsole) = 0xC3;
	SetConsoleTitle("roblox addy grabber");
	main();
}

int __stdcall DllMain(HMODULE DLL, DWORD Reason, void* Useless) {
	if (Reason == 1) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)CreateConsole, 0, 0, 0);
	}
	return TRUE;
}