#pragma once
#include <Winsock2.h>

namespace war3_hook {

/**
 * 初始化配置信息
 */
void init(HINSTANCE instance);

// -----------------------------------------------------------------------------------------
// 针对war3的Hook函数
/**
 * Winsock中connect函数的HOOK版本，应用于TCP数据转发，此函数内会将s设置为阻塞模式
 */
SOCKET __stdcall connect(SOCKET s, const sockaddr* name, int namelen);

/**
 * Winsock中sendto函数的HOOK版本，应用于UDP数据转发
 */
int __stdcall sendto(SOCKET s, const char* buf, int len, int flags, const sockaddr* to, int tolen);

/**
 * Winsock中recvfrom函数的HOOK版本，应用于UDP数据转发
 */
int __stdcall recvfrom(SOCKET s, char* buf, int len, int flags, sockaddr* from, int* fromlen);

} // namespace war3_hook