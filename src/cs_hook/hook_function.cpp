#include "hook_function.h"
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

namespace cs_hook {

SOCKET __stdcall WSASocketA(int af, int type, int protocol, LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP g, DWORD dwFlags) {
	std::cout << __FUNCTION__ 
			  << af << ","
			  << type << ","
			  << protocol << ","
			  << std::hex << (void*)lpProtocolInfo << "," << std::dec
			  << ","
			  << dwFlags << ","
			  << "-sogp2" << std::endl;
	return ::WSASocketA(af, type, protocol, lpProtocolInfo, g, dwFlags);
}

SOCKET __stdcall socket(int af, int type, int protocol) {
	std::cout << __FUNCTION__
			  << af << ","
			  << type << ","
			  << protocol << ","
			  << "-sogp2" << std::endl;
	return ::socket(af, type, protocol);
}

}