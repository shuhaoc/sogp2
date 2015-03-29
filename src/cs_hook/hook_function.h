#pragma once
#include <Winsock2.h>

namespace cs_hook {

SOCKET __stdcall WSASocketA(int af, int type, int protocol, LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP p, DWORD dwFlags);

SOCKET __stdcall socket(int af, int type, int protocol);

int __stdcall WSAConnect(SOCKET s, const sockaddr* name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS);

} // namespace cs_hook