#include "../utility/apihook/apihook.h"
#include "../utility/debugging/debugstream.h"
#include "hook_function.h"

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {
	switch(reason) {
		case DLL_PROCESS_ATTACH:
			// cout重写向至调试器
			std::cout.rdbuf(utility::debug.rdbuf());
			// 初始化配置项
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
	// 别忘记返回TRUE，否则调用者将加载失败。
	return TRUE;
}