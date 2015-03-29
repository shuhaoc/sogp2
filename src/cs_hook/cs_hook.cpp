#include "../utility/apihook/apihook.h"
#include "../utility/debugging/debugstream.h"
#include "hook_function.h"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
	switch(reason) {
		case DLL_PROCESS_ATTACH:
			// cout��д����������
			std::cout.rdbuf(utility::debug.rdbuf());
			// ��ʼ��������
			//war3_hook::init(instance);
			break;
			
		case DLL_THREAD_ATTACH:
			HMODULE winsock_dll = LoadLibrary("ws2_32.dll");
			try {
				utility::apihook::InstallHookForThisThread(winsock_dll, "WSASocketA", cs_hook::WSASocketA);
				utility::apihook::InstallHookForThisThread(winsock_dll, "socket", cs_hook::socket);
			} catch (std::exception& ex) {
				std::cout << ex.what() << "-sogp2" << std::endl;
				return FALSE;
			}
			break;
	}
	// �����Ƿ���TRUE����������߽�����ʧ�ܡ�
	return TRUE;
}